#include "King.h"

void King::hill_log(Team team)
{
    King::hill_logs.push((int) team);
}

void King::update()
{
    // Count contacts
    King::hill_logs.count_elements();

    // Top occupants hills from team this round 
    Team current_occupant = (Team) King::hill_logs.top_occurence();

    King::global_log_teams[(int)current_occupant]++;


    /*
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
    */

    send_xbee(build_payload(3,(byte)King::global_log_teams[1],(byte)King::global_log_teams[2]));

    /*
           payload hPC;
            hPC.type = 2;
            hPC.message = sf.hill_team0_connected;
            hPC.message2 = sf.hill_team1_connected;

            payload g;
            g.type = 3;
            
            g.message = sf.king_get_team_status(0);
            g.message2 = sf.king_get_team_status(1);
    */

}

void King::read_payload(payload p)
{
    if (p.type == 1)
    {
        King::hill_log((Team)p.message);
    }
}

void King::send_ini()
{
    send_radio(build_payload(4,highByte(GLOBAL_POINTS_MAX),lowByte(GLOBAL_POINTS_MAX)));
}
