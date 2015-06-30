#include "Hill.h"

void Hill::contact_event(Team team)
{
    Hill::contacts.push((int)team);
}

void Hill::update()
{
    // contacts since last update
    if(Hill::contacts.count() != 0)
    {
        // Count contacts
        Hill::contacts.count_elements();

        // Save discrete team steps
        Hill::current_connected_team_red = Hill::contacts.counter[(int)Red];
        Hill::current_connected_team_blue = Hill::contacts.counter[(int)Blue];

        // Top connected players this round 
        Hill::current_occupant = (Team) Hill::contacts.top_occurence();

    }

    Hill::local_log_teams[(int)Hill::current_occupant]++;
    Hill::contacts.empty();

    // send hill occupant status
    send_xbee(build_payload(1,(byte)Hill::current_occupant,0));
    send_radio(build_payload(2,(byte)Hill::current_connected_team_blue, (byte)Hill::current_connected_team_red));
    //send_radio(build_payload(3, 3, 4));
    send_radio(build_payload(3,(byte)Hill::team0_global_status, (byte)Hill::team1_global_status));
    id_pointer = 0;
}

void Hill::read_payload(payload p)
{
    if (p.type == 0)
    {
        bool exist = false;
        for(byte i=0; i<id_pointer; i++)
        {
            if(Hill::player_ids[i] == p.message2)
                exist = true;
        }
        if(!exist)
        {
            Hill::contact_event((Team)p.message);
            Hill::player_ids[id_pointer] = p.message2;
            id_pointer++;
        }
    }
    else if (p.type == 3)
    {
        Hill::team0_global_status = p.message; 
        Hill::team1_global_status = p.message2; 
    }
}
