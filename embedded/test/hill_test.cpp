#include <iostream>
#include <assert.h>
#include "helper.h"

#include "Hill.h"

void test_hill()
{
    PACKAGE("Hill Class")

    Hill h;

    assert(h.current_occupant == Neutral);
    assert(h.current_connected_team_red == 0);
    assert(h.current_connected_team_blue == 0);

    h.contact_event(Red);
    h.contact_event(Red);
    h.contact_event(Blue);
    h.contact_event(Blue);
    h.contact_event(Blue);

    h.update();

    assert(h.current_occupant == Blue);
    assert(h.current_connected_team_red == 2);
    assert(h.current_connected_team_blue == 3);

    PACKAGE_END("Successfully tested.")
}


