#include <iostream>
#include <assert.h>

#include "helper.h"

#include "Hill.h"
#include "King.h"
#include "enum.h"


// Tests
void test_stack();
void test_hill();
void test_king();
void test_player();

int main ()
{
    test_stack();
    test_hill();
    test_king();
    test_player();

    return 0;
}


void test_king()
{
    King k;

    k.hill_log(Red);
    k.hill_log(Red);
    k.hill_log(Red);
    k.hill_log(Blue);
    k.hill_log(Blue);

    k.update();
}

void test_hill()
{
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
}


