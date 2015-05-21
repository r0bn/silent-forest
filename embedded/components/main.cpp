#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include "SF.h"
#include "def.h"

/*
 * Message Types:
 * 0 : Player Ping
 * 1 : Hill Ping
 */
struct payload
{
  byte type;
  byte message;
};

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

byte addresses[][6] = {"1Node","2Node"};

// Set up roles to simplify testing 
boolean role;                                    // The main role variable, holds the current role identifier
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.

#ifdef PLAYER
unsigned long last_send = 0;
#endif

#ifdef HILL
unsigned long hill_last_send = 0;
#endif

SF sf;

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
  
#ifdef HILL
    radio.startListening();                 // Start listening
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
        if (!radio.write( &p , sizeof(payload) ))
        {  
             Serial.println("failed.");  
        }

        last_send = millis();
    }
#endif


#ifdef HILL
    // wait for player contact pings
    if(radio.available())
    {
        payload p;
        while (radio.available()) 
        {                                   // While there is data ready
            radio.read( &p, sizeof(payload) );             // Get the payload
        }    

        if(p.type == 0)
        {
            //Serial.print("Ping from team: ");  
            //Serial.println(p.message);
            sf.hill_contact_event(p.message);
        }

#ifdef KING
        // receive and handle hill status messages
        if(p.type == 1)
        {
            Serial.print("Hill Status: ");  
            Serial.println(p.message);

            // TODO: Cleanup workaround 3 as neutral
            if(p.message != 3)
            {
                sf.king_log_event(p.message);
            }
        }
#endif
    }

    if(HILL_FREQUENCY_MS < (millis() - hill_last_send))
    {
        sf.hill_update();

#ifndef KING
        // send hill occupant status if team is dominant
        payload p;
        p.type = 1;
        if(sf.hill_current_occupant > -1)
            p.message = sf.hill_current_occupant;
        else
            p.message = 3;

        radio.stopListening();
        radio.write(&p, sizeof(payload));
        radio.startListening();

        Serial.print("Send occupant: ");  
        Serial.println(p.message);
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

        Serial.print("King Team0: ");
        Serial.println(sf.king_get_team_status(0));
        Serial.print("King Team1: ");
        Serial.println(sf.king_get_team_status(1));

#endif
        hill_last_send = millis();
    }
#endif

