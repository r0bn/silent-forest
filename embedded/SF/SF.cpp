#include <iostream>
#include "SF.h"

void SF::hill_contact_event(int teamId)
{
    hill_contacts[SF::hill_contact_pointer] = teamId;
    SF::hill_contact_pointer++;
}

void SF::hill_update()
{
    int counter1 = 0;
    int counter2 = 0;

    while(SF::hill_contact_pointer > 0) 
    {
        if(hill_contacts[SF::hill_contact_pointer-1] == TEAM_01) 
        {
            counter1++;
            D("counter1++")
        }

        if(hill_contacts[SF::hill_contact_pointer-1] == TEAM_02) 
        {
            counter2++;
            D("counter2++")
        }

        SF::hill_contact_pointer--;
    }

    if(counter1 > counter2)
    {
        SF::hill_occupant_teamId = TEAM_01;
    }
    else if(counter2 > counter1)
    {

        SF::hill_occupant_teamId = TEAM_02;
    }
}
