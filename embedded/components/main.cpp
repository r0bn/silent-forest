#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include "SF.h"
#include "def.h"

/*
 * Message Types:
 * 0 : Player Ping
 * 1 : Hill Ping
 * 2 : Team Hill Status for Player
 * 3 : Team Global Status for Player
 */
struct payload
{
  byte type;
  byte message;
  byte message2;
};

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

byte addresses[][6] = {"1Node","2Node"};

// Set up roles to simplify testing 
boolean role;                                    // The main role variable, holds the current role identifier
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.

#ifdef PLAYER
unsigned long last_send = 0;
byte team0_hill_status = 0;
byte team1_hill_status = 0;
byte team0_global_status = 0;
byte team1_global_status = 0;
#endif

#ifdef HILL
unsigned long hill_last_send = 0;
byte team0_global_status = 0;
byte team1_global_status = 0;
#endif

SF sf;

void updateLedState(int team0, int team1);
void resetLED();

void setup() 
{
    Serial.begin(57600);
    Serial.println();

#ifdef HILL 
    /*
     *  This component is a hill
     */
    Serial.println("Role Hill");
#endif

#ifdef PLAYER 
/*
 *  This component is a player with a defined team
 */
  Serial.println("Role Player");
  Serial.println("Team: ");
  Serial.println(TEAM);
#endif

#ifdef KING 
/*
 *  This component is a king additional to hill role and handles the global state 
 */
  Serial.println("Role King");
#endif

    // Setup and configure rf radio
    radio.begin();                          // Start up the radio
    radio.setAutoAck(1);                    // Ensure autoACK is enabled
    radio.setRetries(15,15);                // Max delay between retries & number of retries
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  
    radio.startListening();                 // Start listening

#ifdef PLAYER 
    resetLED();
    // Status Hill connection
    pinMode(8,OUTPUT);
    digitalWrite(8,LOW);
    // Button Status
    pinMode(A5,INPUT);
    digitalWrite(A5,HIGH);
#endif

#ifdef XBEE
    Serial1.begin(115200);
#endif
}

void loop(void)
{
#ifdef PLAYER
    // Send a frequent ping  
    if(FREQUENCY_MS < (millis() - last_send))
    {
        payload p;
        p.type = 0;
        p.message = TEAM; 
    
        Serial.println("Now sending");
        radio.stopListening();
        if (!radio.write( &p , sizeof(payload) ))
        {  
            digitalWrite(8,LOW);
            Serial.println("failed.");  
        }
        else
        {
            digitalWrite(8,HIGH);
        }
        radio.startListening();


        last_send = millis();
    }
#endif

#ifdef XBEE
    if(Serial1.available() > 3)
    {
        if(Serial1.read() == 'a')
        {
            byte type = Serial1.read();
            byte message = Serial1.read();
            byte message2 = Serial1.read();

#ifdef KING
            if(type == 1)
            {
                Serial.print("Hill Status: ");  
                Serial.println(message);

                if(message != 3)
                {
                    sf.king_log_event(message);
                }
            }
#endif

#ifdef HILL
            if(type == 3)
            {
                Serial.println("Received Global state");  
                team0_global_status = message; 
                team1_global_status = message2; 
            }

#endif
        }
    }
#endif

    // wait for player contact pings
    if(radio.available())
    {
        payload p;
        while (radio.available()) 
        {                                   // While there is data ready
            radio.read( &p, sizeof(payload) );             // Get the payload
        }    

#ifdef HILL

        if(p.type == 0)
        {
            //Serial.print("Ping from team: ");  
            //Serial.println(p.message);
            sf.hill_contact_event(p.message);
        }
#endif

#ifdef PLAYER

        if(p.type == 2)
        {
            team0_hill_status = p.message;
            team1_hill_status = p.message2;
        }

        if(p.type == 3)
        {
            team0_global_status = p.message;
            team1_global_status = p.message2;
        }
        if(digitalRead(A5) == LOW)
        {
            byte t0 = 0;
            byte t1 = 0;
            if(team0_global_status > 0)
                t0 = (int)(team0_global_status / GLOBAL_POINTS_MAX * 5.0);
            if(team1_global_status > 0)
                t1 = (int)(team1_global_status / GLOBAL_POINTS_MAX * 5.0);

            if(t0 > 5)
                t0 = 5;
            if(t1 > 5)
                t1 = 5;

            updateLedState(t0, t1);
        }
        else 
        {
            updateLedState(team0_hill_status, team1_hill_status);
        }

#endif


    }

#ifdef HILL
    if(HILL_FREQUENCY_MS < (millis() - hill_last_send))
    {
        sf.hill_update();


#ifndef KING
        // send hill occupant status 
#ifdef XBEE

        byte message = 0;
        if(sf.hill_current_occupant > -1)
            message = sf.hill_current_occupant;
        else
            message = 3;
        Serial1.write('a');
        Serial1.write(1);
        Serial1.write(message);
        Serial1.write(0);

        Serial.print("Send occupant: ");  
        Serial.println(message);
#endif

#else
        // if king role, not send hill state on air
        Serial.print("King occupant: ");  
        Serial.println(sf.hill_current_occupant);

        if(sf.hill_current_occupant > -1)
            sf.king_log_event(sf.hill_current_occupant);


#endif

#ifdef KING
        // update global statek
        sf.king_update();

#ifdef XBEE
        Serial1.write('a');
        Serial1.write(3);
        Serial1.write(sf.king_get_team_status(0));
        Serial1.write(sf.king_get_team_status(1));
#endif

        Serial.print("King Team0: ");
        Serial.println(sf.king_get_team_status(0));
        Serial.print("King Team1: ");
        Serial.println(sf.king_get_team_status(1));
#endif

        payload hPC;
        hPC.type = 2;
        hPC.message = sf.hill_team0_connected;
        hPC.message2 = sf.hill_team1_connected;

        payload g;
        g.type = 3;

#ifndef KING
        g.message = team0_global_status;
        g.message2 = team1_global_status;
#else
        g.message = sf.king_get_team_status(0);
        g.message2 = sf.king_get_team_status(1);
#endif

        radio.stopListening();
        radio.write(&hPC, sizeof(payload));
        radio.write(&g, sizeof(payload));
        radio.startListening();

        hill_last_send = millis();
    }
#endif
}



#ifdef PLAYER
void resetLED() 
{
    // RED
    for(int i=2;i<7;i++)
    {
        pinMode(i,OUTPUT);
        digitalWrite(i,LOW);
    }
    // Blue
    for(int i=0;i<5;i++)
    {
        pinMode(A4 - i,OUTPUT);
        digitalWrite(A4 - i,LOW);
    }
}
#endif

#ifdef PLAYER
void updateLedState(int team0, int team1)
{
    resetLED();

    // RED
    for(int i=2;i<(2+team0);i++)
    {
        digitalWrite(i,HIGH);
    }
    // Blue
    for(int i=0;i<(0+team1);i++)
    {
        digitalWrite(A4 - i,HIGH);
    }

}
#endif

