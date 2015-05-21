#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#include "printf.h"

#include "SF.h"
#include "def.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

byte addresses[][6] = {"1Node","2Node"};


// Set up roles to simplify testing 
boolean role;                                    // The main role variable, holds the current role identifier
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.

#ifdef PLAYER
unsigned long last_send = 0;
#endif

SF sf;

void setup() {


    Serial.begin(57600);
    //printf_begin();

#ifdef HILL 
  Serial.println("\nRole Hill");
#endif

#ifdef PLAYER 
  Serial.println("\nRole Player");
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
  //radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void)
{

#ifdef PLAYER
    
    if(FREQUENCY_MS < (millis() - last_send))
    {
    
        byte team = TEAM;
        Serial.println("Now sending");
        if (!radio.write( &team , sizeof(byte) ))
        {  
             Serial.println("failed.");  
        }

        last_send = millis();
    }
#endif


#ifdef HILL

    if(radio.available())
    {
        byte team;
        while (radio.available()) 
        {                                   // While there is data ready
            radio.read( &team, sizeof(byte) );             // Get the payload
        }    
     
        Serial.print("Received team: ");  
        Serial.println(team);

        sf.hill_contact_event(team);

        Serial.print("Team0:");
        Serial.println(sf.hill_get_team_status(0));
        Serial.print("Team1:");
        Serial.println(sf.hill_get_team_status(1));
    }

    sf.hill_update_freq(millis());

#endif


/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      Serial.println("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      role = role_ping_out;                  // Become the primary transmitter (ping out)
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
  
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      Serial.println("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");
      
       role = role_pong_back;                // Become the primary receiver (pong back)
       radio.openWritingPipe(addresses[1]);
       radio.openReadingPipe(1,addresses[0]);
    }
  }
}
