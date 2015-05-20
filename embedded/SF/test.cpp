#include <iostream>

#include "SF.h"

int main ()
{
    SF sf;

    /*
    sf.hill_contact_event(0);
    sf.hill_contact_event(0);
    sf.hill_contact_event(1);

    sf.hill_contact_event(1);
    sf.hill_contact_event(1);
    sf.hill_contact_event(1);

    sf.hill_update_freq(1020);

    std::cout << sf.hill_occupant_teamId << std::endl;
    */

    sf.king_log_event(0);
    sf.king_log_event(0);
    sf.king_log_event(1);
    sf.king_update_freq(1020);

    sf.king_log_event(0);
    sf.king_log_event(1);
    sf.king_log_event(1);
    sf.king_update_freq(3020);


    std::cout << sf.king_get_team_status(0) << std::endl;
    std::cout << sf.king_get_team_status(1) << std::endl;


    return 0;
}

