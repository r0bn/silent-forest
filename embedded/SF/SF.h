// https://github.com/maniacbug/RF24/commit/de083c964d9aeeb9fda7485c39fed27443cd617c

#ifndef __SF_H__
#define __SF_H__

#define MAX_CONTACTS 20

#define TEAM_01 10
#define TEAM_02 20

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

    protected:

    public:
        /**
         * Constructor
         */
        SF(void) :
            hill_contact_pointer(0),
            hill_occupant_teamId(0),
            hill_contacts()
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
         * hold's the current teamId of the hill occupant
         */
        int hill_occupant_teamId;

};

#endif