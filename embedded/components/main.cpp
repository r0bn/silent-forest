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
unsigned long get_millis();
void set_global_led();
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
void update_king();
unsigned long last_send_king = 0;
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
    Serial1.begin(9600);
#endif
}

void loop(void)
{
#ifdef XBEE
    read_xbee();
#endif
    read_radio();


#ifdef PLAYER
    update_player();
#endif

#ifdef HILL
    update_hill();
#endif


#ifdef KING
    update_king();
#endif

}
unsigned long last_ping = 0;
#ifdef PLAYER
void update_player()
{
        player.ping(millis());

        
    if(player.gameStatus == INIT)
    {
        io.setModeTeam(PULSE);
        if(io.buttons[0].state == DOWN)
        {
            if(player.teamId == Blue)
            {
                player.teamId = Red;
            }

            else
            {
                player.teamId = Blue;
            }
        }
        if(player.teamId == Blue || player.teamId == Red)
        {
            if(player.teamId == Blue)
            {
                io.setHillTeam(5, 0);
            }
            else
            {
                io.setHillTeam(0, 5);
            }
        }
        else
        {
            io.setHillTeam(5, 5);
        }
    }

    if(player.gameStatus == START)
    {
        if((millis() - player.prelude_start_time) < 5000)
        {
            io.setModeTeam(ON);
        }
        else
        {
            unsigned int tmp = map(millis() - player.prelude_start_time - 5000, 0, player.prelude_period - 5000, 0, 5);
            if(tmp < 6)
            {
                io.setHillTeam(tmp, tmp);
            }
            else
            {
                io.setHillTeam(0, 0);
                player.gameStatus = PLAY;
                delay(random(0,750));
            }
        }
    }

    if(player.gameStatus == PLAY)
    {
        io.setHillTeam(player.team0_hill_status, player.team1_hill_status);
        io.setGlobalTeam(player.team0_global_status, player.team1_global_status);
        
        io.setModeTeam(ON);

        if((millis() - player.last_receive) > 2000)
        {
            io.setStatus1(OFF);
            io.setHillTeam(0,0);
        }
        else
        {
            io.setStatus1(ON);
        }

        if(io.buttons[0].state == DOWN)
        {
            io.switchTo(TEAM_GLOBAL);
        }
        else if(io.buttons[0].state == RELEASED)
        {
            io.switchTo(TEAM_HILL);
        }

    }

    if(player.gameStatus == END)
    {
        io.switchTo(TEAM_GLOBAL);
        io.setModeTeam(PULSE);
    }

    player.update();
    io.update();

}
#endif

#ifdef HILL
void update_hill()
{

    io.update();

    if(FREQUENCY_MS < (millis() - last_send))
    {
        hill.update();
        last_send = millis();
#ifdef KING
        if(hill.current_occupant != Neutral)
        {
            king.hill_log(hill.current_occupant);
        }
#endif
    }
}
#endif

#ifdef KING
void update_king()
{

    if(io.buttons[0].state == DOWN) {
        king.send_ini();
    }

    if(FREQUENCY_MS < (millis() - last_send_king))
    {
        // if king role, not send hill state on air
        king.update();
        last_send_king = millis();
        //Serial.print("king occ: ");
        //Serial.println(king.tmp_top_occ);
        //Serial.print("king points: ");
        //Serial.print(king.global_log_teams[Blue]);
        //Serial.print("\t");
        //Serial.println(king.global_log_teams[Red]);
    }
}
#endif

#ifdef PLAYER
unsigned long get_millis()
{
    return millis();
}
#endif

#ifdef KING
void set_local_global(byte blue, byte red)
{
    hill.team0_global_status = blue;
    hill.team1_global_status = red;
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
            if(p.type == 1)
            {
                Serial.print("hill occ: ");
                Serial.println(p.message);
            }

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

#ifdef KING

        king.read_payload(p);

        if(p.type == 5)
        {
            bool exist = false;
            for(byte i=0; i<king.id_pointer; i++)
            {
                if(king.player_ids[i] == p.message)
                    exist = true;
            }
            if(exist)
            {
                send_radio(build_payload(5,p.message,0));

                Serial.print("0#");
                Serial.println(p.message);
            }
            else
            {
                king.player_ids[king.id_pointer] = p.message;
                king.id_pointer++;

                send_radio(build_payload(5,p.message,1));

                Serial.print("1#");
                Serial.println(p.message);
            }

            for(byte i=0; i<king.id_pointer; i++)
            {
                Serial.print(king.player_ids[i]);
                Serial.print(", ");
            }

        }
#endif

#ifdef PLAYER
        player.read_payload(p);

        if(p.type == 5)
        {
            if(player.gameStatus == INIT && player.id_ok == 0)
            {
                if(p.message == player.Id && p.message2 == 1)
                {
                    player.id_ok = 1;
                }
                if(p.message == player.Id && p.message2 == 0)
                {
                    player.Id = random(255);
                }
            }
        }
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
