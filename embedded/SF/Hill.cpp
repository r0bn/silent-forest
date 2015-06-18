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
}
