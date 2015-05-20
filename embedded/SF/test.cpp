#include <iostream>

#include "SF.h"

int main ()
{
    SF sf;

    sf.hill_contact_event(10);
    sf.hill_update();

    std::cout << sf.hill_occupant_teamId << std::endl;
    return 0;
}

