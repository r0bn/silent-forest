#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
// Needs to be referenced here to use it in the SF library
#include "EEPROM.h"

#include "def.h"
#include "enum.h"
#include "send.h"

#include "IO.h"
#include "SF.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);
const uint64_t addresses[2] = {0xF0F0F0F0E1LL,0xF0F0F0F0D2LL};

// game loop
unsigned long last_send = 0;

#ifdef PLAYER
#include "Player.h"
void update_player();
Player player;
IO io(PLAYER_ROLE);
#endif

#ifdef HILL
#include "Hill.h"
void update_hill();
Hill hill;
IO io(HILL_ROLE);
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

    //Get random seed from A5 to generate random id
    digitalWrite(A5, LOW);
    delay(20);
    randomSeed(analogRead(A5));
    player.Id = random(255);
    player.ping_frequency = random(500,750);
    digitalWrite(A5, HIGH);
    Serial.print("Player Id: ");
    Serial.println(player.Id);
    delay(3000);
#endif

#ifdef KING 
    Serial.println("Role King");
#endif

    // Setup and configure rf radio
    radio.begin();                          // Start up the radio
#ifndef PLAYER
    radio.setAutoAck(1);                    // Ensure autoACK is enabled
#endif
    radio.setRetries(15,15);                // Max delay between retries & number of retries
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  
    radio.startListening();                 // Start listening
    radio.setPayloadSize(4);

#ifdef XBEE
    Serial1.begin(115200);
#endif
}

void loop(void)
{
#ifdef XBEE
    //read_xbee();
#endif
    read_radio();


#ifdef PLAYER
        update_player();
#endif

#ifdef HILL
        update_hill();
#endif


#ifdef KING
        // if king role, not send hill state on air
        king.hill_log(hill.current_occupant);
        
        if(io.buttons[0].state == DOWN) {
            king.send_ini();
        }

        king.update();
#endif

}
unsigned long last_ping = 0;
void send_ping(unsigned long ping_ms)
{
    Serial.print("ping send: ");
    Serial.println(ping_ms);
}
#ifdef PLAYER
void update_player()
{
        io.setHillTeam(player.team0_hill_status, player.team1_hill_status);
        //io.setHillTeam(3,4);
        io.setModeTeam(ON);
        io.update();
        player.update();
        player.ping(millis());
        //Serial.println(player.last_success_ping);

}
#endif

#ifdef HILL
void update_hill()
{

    if(FREQUENCY_MS < (millis() - last_send))
    {
        for(byte i=0; i<hill.id_pointer; i++)
        {
            Serial.print(hill.player_ids[i]);
            Serial.print("\t");
        }
        Serial.println("");
        Serial.println(hill.id_pointer);
        hill.update();
        Serial.print("blue: ");
        Serial.print(hill.current_connected_team_blue);
        Serial.print("\t red: ");
        Serial.println(hill.current_connected_team_red);
        last_send = millis();
    }

}
#endif

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
        io.setStatus1(OFF);
    }
    else
    {
        io.setStatus1(ON);
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
payload static_payload;

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
