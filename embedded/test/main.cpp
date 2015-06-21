#include <iostream>
#include <assert.h>

#include "helper.h"

#include "Player.h"
#include "Hill.h"
#include "King.h"
#include "Stack.h"
#include "StackCounter.h"
#include "enum.h"

#define PRINT(x) std::cout << x << std::endl;

// Tests
void test_stack();
void test_stackCounter();
void test_hill();
void test_king();
void test_player();


int main ()
{
    test_stack();
    test_stackCounter();
    test_hill();
    test_king();
    test_player();

    return 0;
}

void test_player()
{
    Player p; 

    assert(p.gameStatus == INIT);
    assert(p.teamId == -1);

    p.teamId = 1;

    p.ping(100);
    assert(called_send_radio() == 0);

    p.gameStatus = PLAY;
    p.ping(100);
    assert(called_send_radio() == 1);

    std::cout << "Player successfully tested." << std::endl;
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

    std::cout << "King successfully tested." << std::endl;
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

    std::cout << "Hill successfully tested." << std::endl;
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
    //PRINT(stackC.max_amount(5))
    assert(stackC.top_occurence() == 1);

    std::cout << "StackCounter successfully tested." << std::endl;
}

void test_stack()
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

    std::cout << "Stack successfully tested." << std::endl;

}
