/*
 * Player 
 */
#include "enum.h"
#include "send.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player
{
    private:
        /*
         * last success ping
         */
        unsigned long last_success_ping;

    protected:

    public:
        /*
         * Constructor
         */
        Player(void) :
            last_success_ping(0),
            gameStatus(INIT),
            teamId(-1)
        {}

        /* 
         * Player GameStatus
         */
        GameStatus gameStatus;

        /*
         * Team Id
         */
        int teamId;

        /*
         * update player, led's etc...
         */
        void update();

        /*
         * send ping
         */
        void ping(unsigned long millis);
};

#endif
