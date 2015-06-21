#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// Needs to be referenced here to use it in the SF library
#include "EEPROM.h"

#include "def.h"
#include "enum.h"
#include "send.h"

#include "SF.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};

// game loop
unsigned long last_send = 0;

#ifdef PLAYER
#include "Player.h"
Player player;
#endif

#ifdef HILL
#include "Hill.h"
Hill hill;
#endif

#ifdef KING
#include "King.h"
King king;
#endif

#ifdef XBEE
unsigned long last_receive_xbee = 0;
void read_xbee();
#endif

void read_radio();

void setup() 
{
    Serial.begin(57600);
    Serial.println();

#ifdef HILL 
    Serial.println("Role Hill");
#endif

#ifdef PLAYER 
    Serial.println("Role Player");
    Serial.println("Team: ");
    Serial.println(TEAM);
#endif

#ifdef KING 
    Serial.println("Role King");
#endif

    // Setup and configure rf radio
    radio.begin();                          // Start up the radio
    radio.setAutoAck(1);                    // Ensure autoACK is enabled
    radio.setRetries(15,15);                // Max delay between retries & number of retries
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  
    radio.startListening();                 // Start listening

#ifdef XBEE
    Serial1.begin(115200);
#endif
}

void loop(void)
{
#ifdef XBEE
    read_xbee();
#endif
    read_radio();

    // Send a frequent ping  
    if(FREQUENCY_MS < (millis() - last_send))
    {

#ifdef PLAYER
        player.update();
        player.ping(millis());
#endif

#ifdef HILL
        hill.update();
#endif


#ifdef KING
        // if king role, not send hill state on air
        king.hill_log(hill.current_occupant);
        
        if(digitalRead(A4) == LOW) {
            king.send_ini();
        }

        king.update();
#endif

        last_send = millis();

    }
    //led.update();
}

#ifdef XBEE
void read_xbee()
{
    if(Serial1.available() > 3)
    {
        if(Serial1.read() == 'a')
        {
            payload p;
            p.type = Serial1.read();
            p.message = Serial1.read();
            p.message2 = Serial1.read();
    
            last_receive_xbee = millis();

#ifdef KING
            king.read_payload(p);
#endif

#ifdef HILL
            hill.read_payload(p);
#endif
        }
    }

    if(2000 < (millis() - last_receive_xbee))
    {
        digitalWrite(A1,LOW);
        led.setStatus1(OFF)
    }
    else
    {
        digitalWrite(A1,HIGH);

    }
}

#endif

void read_radio()
{
    if(radio.available())
    {
        payload p;
        while (radio.available())           //TODO: While vs if 
        {                                   
            radio.read( &p, sizeof(payload) );   
        }    

#ifdef HILL
        hill.read_payload(p);
#endif

#ifdef PLAYER
        player.read_payload(p);
#endif
    }
}

// Implementation send.h

#ifdef XBEE
void send_xbee(payload p)
{
    Serial1.write('a');
    Serial1.write(p.type);
    Serial1.write(p.message);
    Serial1.write(p.message2);
}
#endif

int send_radio(payload p)
{
    radio.stopListening();
    int ret = radio.write(&p, sizeof(payload));
    radio.startListening();
    return ret;
}

payload build_payload(byte type, byte message, byte message2)
{
    static_payload.type = type;
    static_payload.message = message;
    static_payload.message2 = message2;
    return static_payload;
}
