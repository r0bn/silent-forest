/*
 * HILL 
 */
#include "StackCounter.h"
#include "enum.h"
#include "global.h"
#include "send.h"

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

        /*
         * saves the id's from the players
         */

    protected:

    public:
        /*
         * Constructor
         */
        Hill(void) :
            local_log_teams(),
            current_occupant(Neutral),
            current_connected_team_red(0),
            current_connected_team_blue(0),
            team0_global_status(0),
            team1_global_status(0),
            id_pointer(0)
        {}

        byte id_pointer;
        byte player_ids[MAX_TEAMS * 10];
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

        /*
         * read Radio payloads
         */
        void read_payload(payload p);

        /*
         * Point States
         */
        int team0_global_status;
        int team1_global_status; 

};

#endif
