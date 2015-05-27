// https://github.com/maniacbug/RF24/commit/de083c964d9aeeb9fda7485c39fed27443cd617c

/*
 * hill_contact: An valid received ping from a player by the hill
 * king_log: An valid log message from a hill by the king (central/main counter)
 */

#ifndef __SF_H__
#define __SF_H__

#define MAX_CONTACTS 40
#define MAX_LOGS 40
#define MAX_TEAMS 2

#define UPDATE_FREQUENCY_MS 1000

#define TEAM_01 0
#define TEAM_02 1

#define D(x)

class SF
{
    private:
        /*
         * Pointer for array position for the next contact
         */
        int hill_contact_pointer;
        /*
         * Stack that hold's the contacts since the last update
         */
        int hill_contacts[MAX_CONTACTS];
        /*
         * Last Update Call in millis
         */
        int hill_last_update;
        /*
         * saves how many frequences the player occupant the hill
         */
        int hill_occupant_teams[MAX_TEAMS];

        /*
         * Pointer for array position for the next log
         */
        int king_log_pointer;
        /*
         * Stack that hold's the logs since the last update
         */
        int king_logs[MAX_LOGS];
        /*
         * Last Update Call in millis
         */
        int king_last_update;
        /*
         * saves how many frequences the player has the most hills 
         */
        int king_majority_teams[MAX_TEAMS];


    protected:

    public:
        /**
         * Constructor
         */
        SF(void) :
            hill_contact_pointer(0),
            hill_last_update(0),
            hill_contacts(),
            hill_occupant_teams(),
            hill_current_occupant(-1),
            hill_team0_connected(0),
            hill_team1_connected(0),

            king_log_pointer(0),
            king_last_update(0),
            king_logs(),
            king_majority_teams()
        {}

        /**
         * Register an contact event on the hill 
         */
        void hill_contact_event(int teamId);

        /**
         * Evaluate the contact events and update the hill state
         */
        void hill_update();
        /*
         * Calls the update function in a defined frequency
         */
        void hill_update_freq(int millis);
        /*
         * Returns the global points from the team
         */
        int hill_get_team_status(int teamId);
        /*
         * Returns the current occupant
         */
        int hill_current_occupant;
        /*
         * Hill connected Player team0
         */
        int hill_team0_connected;
        /*
         * Hill connected Player team1
         */
        int hill_team1_connected;

        /**
         * Register an log event
         */
        void king_log_event(int teamId);
        /**
         * Evaluate the log events and update the global state
         */
        void king_update();
        /*
         * Calls the update function in a defined frequency
         */
        void king_update_freq(int millis);
        /*
         * Returns the global points from the team
         */
        int king_get_team_status(int teamId);


};

#endif
