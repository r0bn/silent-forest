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

    protected:

    public:
        /*
         * Constructor
         */
        Player(void) :
            last_success_ping(0),
            last_5(0),
            gameStatus(INIT),
            teamId(-1),
            global_points_max(10),
            team0_hill_status(0),
            team1_hill_status(0),
            team0_global_status(0),
            team1_global_status(0),
            ping_frequency(450),
            Id(0),
            connected(false),
            prelude_period(25000),
            prelude_start_time(0),
            last_receive(0),
            id_ok(0)
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
         * Player Id
         */
        byte Id;

        /*
         * update player, led's etc...
         */
        void update();

        /*
         * send ping
         */
        void ping(unsigned long millis);

        /*
         * read payloads
         */
        void read_payload(payload p);

        /*
         * Game Point States
         */
        int global_points_max;

        int team0_hill_status;
        int team1_hill_status;
        int team0_global_status;
        int team1_global_status;

        /*
         * last success ping
         */
        unsigned long last_success_ping;

        /*
         * ping frequency
         */
        unsigned int ping_frequency;

        /*
         * connected to hill
         */
        bool connected;

        /*
         * start precedure
         */
        unsigned int prelude_period;
        unsigned long prelude_start_time;

        /*
         * last time connected with hill
         */
        unsigned long last_receive;

        // refactoren by rob
        byte id_ok;
        unsigned long last_5;

};

#endif
