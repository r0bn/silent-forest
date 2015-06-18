/*
 * HILL 
 */
#include "StackCounter.h"
#include "enum.h"
#include "global.h"

#ifndef __HILL_H__
#define __HILL_H__

class Hill
{
    private:

        /*
         * Hold the contacts
         */
        StackCounter contacts;

        /*
         * saves how many frequences the player occupant the hill
         * +1 is for logs of neutral phases
         */
        int local_log_teams[MAX_TEAMS + 1];


    protected:

    public:
        /*
         * Constructor
         */
        Hill(void) :
            local_log_teams(),
            current_occupant(Neutral),
            current_connected_team_red(0),
            current_connected_team_blue(0)
        {}

        /*
         * Register an contact event on the hill 
         */
        void contact_event(Team team);

        /*
         * Evaluate the contact events and update the hill state
         */
        void update();

        /*
         * current occupant
         */
        Team current_occupant;

        /*
         * RED Team connected players
         */
        int current_connected_team_red;

        /*
         * Blue Team connected players
         */
        int current_connected_team_blue;

};

#endif
