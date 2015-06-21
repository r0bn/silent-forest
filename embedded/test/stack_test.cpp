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

    PACKAGE_END("Successfully tested.")
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

void test_stack_base()
{
    Stack stack;

    assert(stack.count() == 0);
    assert(stack.pop() == 0);
    assert(stack.top() == 0);

    stack.push(5);

    assert(stack.count() == 1);
    assert(stack.top() == 5);

    assert(stack.pop() == 5);
    assert(stack.count() == 0);
    assert(stack.top() == 0);

    stack.push(15);
    stack.push(30);
    stack.push(40);

    assert(stack.count() == 3);
    assert(stack.top() == 40);
    assert(stack.pop() == 40);

    assert(stack.count() == 2);
    assert(stack.top() == 30);
    assert(stack.pop() == 30);

    stack.empty();

    assert(stack.count() == 0);
    assert(stack.top() == 0);
    assert(stack.pop() == 0);
}
