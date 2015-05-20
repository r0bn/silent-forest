#include <iostream>

#include "SF.h"

int main ()
{
    SF sf;

    sf.hill_contact_event(0);
    sf.hill_contact_event(0);
    sf.hill_contact_event(1);

    sf.hill_contact_event(1);
    sf.hill_contact_event(1);
    sf.hill_contact_event(1);

    sf.hill_update_freq(1020);

    std::cout << sf.hill_occupant_teamId << std::endl;
    return 0;
}

