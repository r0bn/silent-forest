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
 * 4 : Ini Global Status Message and start
 */
struct payload
{
  byte type;
  byte message;
  byte message2;
};

enum GameStatus
{
    INIT,
    START,
    PLAY,
    END
};

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

byte addresses[][6] = {"1Node","2Node"};

#ifdef PLAYER
unsigned long last_send = 0;
unsigned long last_success_ping = 0;
unsigned long last_pulse = 0;
byte team0_hill_status = 0;
byte team1_hill_status = 0;
byte team0_global_status = 0;
byte team1_global_status = 0;
bool led_pulse_status = LOW;
GameStatus game_status = INIT;
int global_points_max = 5;
#endif

#ifdef HILL
unsigned long hill_last_send = 0;
byte team0_global_status = 0;
byte team1_global_status = 0;
#endif

#ifdef KING
byte status_led_last = 0;
#endif

#ifdef XBEE
unsigned long last_receive_xbee = 0;
#endif

SF sf;

void updateLedState(int team0, int team1);
void resetLED();
void showGlobalStatus();

void setup() 
{
    Serial.begin(57600);
    Serial.println();

#ifdef HILL 
    /*
     *  This component is a hill
     */
    Serial.println("Role Hill");
    
    // Status1
    pinMode(A0,OUTPUT);
    digitalWrite(A0,LOW);

    // Status2 
    pinMode(A1,OUTPUT);
    digitalWrite(A1,LOW);
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

    // Button Status Init Messeage
    pinMode(A4,INPUT);
    digitalWrite(A4,HIGH);
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
    read_xbee();
    read_radio();

    // Send a frequent ping  
    if(FREQUENCY_MS < (millis() - last_send))
    {

#ifdef PLAYER
        player_ping();
        player_update();
#endif

#ifdef HILL

        sf.hill_update();

            g.message = team0_global_status;
            g.message2 = team1_global_status;

        Payload p;
        p.type = 1;
        p.message = 0;
        if(sf.hill_current_occupant > -1)
            p.message = sf.hill_current_occupant;
        else
            p.message = 3;
        send(p,XBEE)


        if(2000 < (millis() - last_receive_xbee))
        {
            digitalWrite(A1,LOW);
            led.setStatus1(OFF)
        }
        else
        {
            digitalWrite(A1,HIGH);

        }
#endif


#ifdef KING
 // if king role, not send hill state on air
        Serial.print("King occupant: ");  
        Serial.println(sf.hill_current_occupant);

        if(sf.hill_current_occupant > -1)
            sf.king_log_event(sf.hill_current_occupant);

        
        // Send ini Message 
        if(digitalRead(A4) == LOW) {

            payload hPC;
            hPC.type = 4;
            hPC.message = highByte(GLOBAL_POINTS_MAX);
            hPC.message2 = lowByte(GLOBAL_POINTS_MAX);

            radio.stopListening();
            radio.write(&hPC, sizeof(payload));
            radio.startListening();

            // do nothin else
        }
        else
        {
            if(sf.king_get_team_status(0) >= GLOBAL_POINTS_MAX || sf.king_get_team_status(1) >= GLOBAL_POINTS_MAX)
            {
                digitalWrite(A0, HIGH);
            }
            else
            {
                status_led_last = !status_led_last;
                digitalWrite(A0, status_led_last);

                // update global statek
                sf.king_update();
            }


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
            
            g.message = sf.king_get_team_status(0);
            g.message2 = sf.king_get_team_status(1);


#endif





        last_send = millis();
    }

    led.update();


    }
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
    // BLUE
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
    // BLUE
    for(int i=0;i<(0+team1);i++)
    {
        digitalWrite(A4 - i,HIGH);
    }

}
#endif

#ifdef PLAYER
void showGlobalStatus()
{
    byte t0 = 0;
    byte t1 = 0;
    if(team0_global_status > 0)
        t0 = map(team0_global_status, 0, global_points_max, 0, 5);
    if(team1_global_status > 0)
        t1 = map(team1_global_status, 0, global_points_max, 0, 5);

    if(t0 > 5)
        t0 = 5;
    if(t1 > 5)
        t1 = 5;

    updateLedState(t0, t1);
}
#endif

void player_ping()
{
    if(game_status == PLAY)
    {
        payload p;
        p.type = 0;
        p.message = TEAM; 
    
        Serial.println("Now sending");
        radio.stopListening();
        if (radio.write( &p , sizeof(payload) ))
        {
            last_success_ping = millis();
        }
        radio.startListening();

        if(2000 < millis() - last_success_ping)
        {
            digitalWrite(8,LOW);
        }
        else
        {
            digitalWrite(8,HIGH);
        }

    }
}

void player_update() 
{
    if(game_status == INIT)
    {
        if(TEAM == 0)
        {
            updateLedState(5, 0);
        }
        else
        {
            updateLedState(0, 5);
        }
    }

    if(game_status == START){
        
        int tmp = map(millis()  - start_time, 0, start_period_time, 0, 5);
        if(tmp < 6)
            updateLedState(tmp, tmp);
        else
        {
            updateLedState(0, 0);
            game_status = PLAY;
        }
    }

    if(game_status == PLAY)
    {
        if(digitalRead(A5) == LOW)
        {
            showGlobalStatus();
        }
        else 
        {
            updateLedState(team0_hill_status, team1_hill_status);
        }
    }

    if(game_status == END)
    {
        if(1000 < millis() - last_pulse)
        {
            if(led_pulse_status)
                showGlobalStatus();
            else
                resetLED();

            last_pulse = millis();
            led_pulse_status = !led_pulse_status;
        }
    }
}

void read_xbee()
{
// ####################################################################################################
// XBEE Receive Communication

#ifdef XBEE
    if(Serial1.available() > 3)
    {
        if(Serial1.read() == 'a')
        {
            byte type = Serial1.read();
            byte message = Serial1.read();
            byte message2 = Serial1.read();
    
            last_receive_xbee = millis();

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

}

void read_radio()
{
// ##################################################################################################
// Funk Receive  

    if(radio.available())
    {
        payload p;
        while (radio.available())           //TODO: While vs if 
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

        if(game_status == INIT)
        {
            if(p.type == 4)
            {
                global_points_max = word(p.message, p.message2);
                game_status = START;
                Serial.println("Start game");
                Serial.println(global_points_max);
            }
        }

        if(team0_global_status >= global_points_max || team1_global_status >= global_points_max)
        {
            game_status = END;
        }

#endif

    }
}

void send(payload p, XBEE/RADIO)
{
    if(XBEE)
    {
        Serial1.write('a');
        Serial1.write(p.type);
        Serial1.write(p.message);
        Serial1.write(p.message2);
    }
    else if(RADIO)
    {
        radio.stopListening();
        radio.write(&p, sizeof(payload));
        radio.startListening();
    }
}
