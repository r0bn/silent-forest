#include <iostream>
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
        SF::hill_occupant_teamId = pointerMax;
    }
}
