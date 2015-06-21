#include <iostream>
#include <assert.h>
#include "helper.h"

#include "King.h"

void test_king()
{
    PACKAGE("Hill Class")

    King k;

    k.hill_log(Red);
    k.hill_log(Red);
    k.hill_log(Red);
    k.hill_log(Blue);
    k.hill_log(Blue);

    k.update();

    PACKAGE_END("Successfully tested.")
}


