#include "SF.h"

void SF::hill_contact_event(int teamId)
{
    hill_contacts[SF::hill_contact_pointer] = teamId;
    SF::hill_contact_pointer++;
}

void SF::hill_update_freq(int millis)
{
    if((millis - SF::hill_last_update) > UPDATE_FREQUENCY_MS)
    {
        SF::hill_update();
        SF::hill_last_update = millis;
    }
}

void SF::hill_update()
{
    int counter[MAX_TEAMS] = {0};

    while(SF::hill_contact_pointer > 0) 
    {
        // get teamId, increment in counter, decrement pointer
        int team = hill_contacts[SF::hill_contact_pointer-1]; 
        counter[team]++;
        SF::hill_contact_pointer--;
    }

    // Save State
    SF::hill_team0_connected = counter[0];
    SF::hill_team1_connected = counter[1];

    // find max detected team player
    int temp = -1;
    int pointerMax = -1;
    for(int i=0;i< MAX_TEAMS;i++)
    {
        // if same as last max, then cancel because draw
        if(counter[i] == temp)
        {
            pointerMax = -1;
            break;
        }

        if(counter[i] > temp)
        {
            temp = counter[i];
            pointerMax = i;
        }

    }

    if(pointerMax > -1)
    {
        SF::hill_occupant_teams[pointerMax]++;
        SF::hill_current_occupant = pointerMax;
    }
}

int SF::hill_get_team_status(int teamId)
{
    return SF::hill_occupant_teams[teamId];
}

void SF::king_log_event(int teamId)
{
    king_logs[SF::king_log_pointer] = teamId;
    SF::king_log_pointer++;
}

void SF::king_update_freq(int millis)
{
    if((millis - SF::king_last_update) > UPDATE_FREQUENCY_MS)
    {
        SF::king_update();
        SF::king_last_update = millis;
    }
}

void SF::king_update()
{
    int counter[MAX_TEAMS] = {0};

    while(SF::king_log_pointer > 0) 
    {
        // get teamId, increment in counter, decrement pointer
        int team = king_logs[SF::king_log_pointer-1]; 
        counter[team]++;
        SF::king_log_pointer--;
    }

    int temp = -1;
    int pointerMax = -1;
    for(int i=0;i< MAX_TEAMS;i++)
    {
        // if same as last max, then cancel because draw
        if(counter[i] == temp)
        {
            pointerMax = -1;
            break;
        }

        if(counter[i] > temp)
        {
            temp = counter[i];
            pointerMax = i;
        }

    }

    if(pointerMax > -1)
    {
        if(counter[0] > counter[1] )
        {
            int diff = counter[0] - counter[1];
            SF::king_majority_teams[pointerMax] += diff;
        }
        if(counter[1] > counter[0] )
        {
            int diff = counter[1] - counter[0];
            SF::king_majority_teams[pointerMax] += diff;
        }
    }
}

int SF::king_get_team_status(int teamId)
{
    return SF::king_majority_teams[teamId];
}

