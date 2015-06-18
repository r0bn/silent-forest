/*
 * King 
 */
#include "StackCounter.h"
#include "enum.h"
#include "global.h"


#ifndef __KING_H__
#define __KING_H__

class King
{
    private:

        /*
         * Hold the hill logs 
         */
        StackCounter hill_logs;

        /*
         * saves the global overal game state
         * +1 is for logs of neutral phases
         */
        int global_log_teams[MAX_TEAMS + 1];


    protected:

    public:
        /*
         * Constructor
         */
        King(void) :
            global_log_teams()
        {}

        /*
         * Register an hill log with the current occupant 
         */
        void hill_log(Team team);

        /*
         * Evaluate the hill logs and update global team state
         */
        void update();

};

#endif
