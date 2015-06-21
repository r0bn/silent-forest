#include <iostream>
#include <assert.h>
#include "helper.h"

#include "Stack.h"
#include "StackCounter.h"

/*
 * Tests the base Stack implementation
 */
void test_stack_base();
/*
 * Tests the stack count implementation
 */
void test_stackCounter();

void test_stack()
{
    PACKAGE("Stack Classes")
    
    test_stack_base();
    test_stackCounter();

    PRINT("Successfully tested.")
}


void test_stackCounter()
{
    StackCounter stackC;

    assert(stackC.top_occurence() == -1);
    
    stackC.count_elements();

    assert(stackC.counter[0] == 0);
    assert(stackC.counter[MAX_ELEMENTS_COUNTABLE - 1] == 0);

    stackC.push(0);
    stackC.push(0);
    stackC.push(1);
    stackC.push(1);
    stackC.push(1);
    stackC.push(4);
    stackC.count_elements();

    assert(stackC.counter[0] == 2);
    assert(stackC.counter[1] == 3);
    assert(stackC.counter[4] == 1);
    assert(stackC.top_occurence() == 1);
}


