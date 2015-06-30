#include "Player.h"

void Player::update()
{
    if(Player::team0_global_status >= Player::global_points_max || Player::team1_global_status >= Player::global_points_max)
    {
        Player::gameStatus = END;
    }
}

void Player::ping(unsigned long millis)
{
    if (Player::gameStatus == PLAY && (millis - Player::last_success_ping) > Player::ping_frequency)
    {
        if (send_radio(build_payload(0,(byte)Player::teamId,Player::Id)))
        {
            //Player::last_success_ping = millis;
        }
        Player::last_success_ping = millis;
    }
}

void Player::read_payload(payload p)
{
    if(p.type == 0)
    {
        if(p.message2 == Player::Id)
            Player::last_success_ping += 1600;
    }
    if(p.type == 2)
    {
        Player::team0_hill_status = p.message;
        Player::team1_hill_status = p.message2;

    }
    else if(p.type == 3)
    {
        Player::team0_global_status = p.message;
        Player::team1_global_status = p.message2;
    }
    else if(p.type == 4)
    {
        if(Player::gameStatus == INIT)
        {
            Player::global_points_max = p.message | p.message2;

            Player::gameStatus = START;
        }
    }
}
