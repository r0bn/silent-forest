// https://github.com/maniacbug/RF24/commit/de083c964d9aeeb9fda7485c39fed27443cd617c

#ifndef __SF_H__
#define __SF_H__

#define MAX_CONTACTS 20
#define MAX_TEAMS 2

#define UPDATE_FREQUENCY_MS 1000

#define TEAM_01 0
#define TEAM_02 1

#define D(x) std::cout << x << std::endl;

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

    protected:

    public:
        /**
         * Constructor
         */
        SF(void) :
            hill_contact_pointer(0),
            hill_occupant_teamId(0),
            hill_last_update(0),
            hill_contacts(),
            hill_occupant_teams()
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
         * hold's the current teamId of the hill occupant
         */
        int hill_occupant_teamId;

};

#endif
