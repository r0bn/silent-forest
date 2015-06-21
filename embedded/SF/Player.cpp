#include "Player.h"

void Player::update()
{

    /*
    if (Player::gameStatus == PLAY)
    {
        if(2000 < millis() - last_success_ping)
        {
            digitalWrite(8,LOW);
        }
        else
        {
            digitalWrite(8,HIGH);
        }
    }

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

    */

}

void Player::ping(unsigned long millis)
{
    if (Player::gameStatus == PLAY)
    {
        if (send_radio(build_payload(0,Player::teamId,0)))
        {
            Player::last_success_ping = millis;
        }

    }

}
