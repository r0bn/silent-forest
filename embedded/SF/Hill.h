/*
 * HILL 
 */
#include "Stack.h"

#ifndef __HILL_H__
#define __HILL_H__

class Hill
{
    private:

        /*
         * Hold's the contacts
         */
        Stack contacts;

    protected:

    public:
        /*
         * Constructor
         */
        Hill(void)
        {}

        /**
         * Register an contact event on the hill 
         */
        void contact_event(int teamId);

        /**
         * Evaluate the contact events and update the hill state
         */
        void update();

        /*
         * Returns the global points from the team
         */
        int get_team_status(int teamId);

        /*
         * Returns the current occupant
         */
        int get_current_occupant();

};

#endif
