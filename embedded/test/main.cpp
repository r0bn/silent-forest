#include <iostream>
#include <assert.h>

#include "Player.h"
#include "Hill.h"
#include "King.h"
#include "Stack.h"


// Test Stack Class
void test_stack();

int main ()
{
    Player p; 
    Hill h;
    King k;

    test_stack();

    
    /*
    sf.hill_contact_event(0);
    sf.hill_contact_event(0);
    sf.hill_contact_event(1);

    sf.hill_contact_event(1);
    sf.hill_contact_event(1);
    sf.hill_contact_event(1);

    sf.hill_update_freq(1020);

    std::cout << sf.hill_get_team_status(0) << std::endl;
    std::cout << sf.hill_get_team_status(1) << std::endl;

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
    */


    return 0;
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

    assert(stack.count() == 1);
    assert(stack.top() == 15);
    assert(stack.pop() == 15);

    assert(stack.count() == 0);
    assert(stack.top() == 0);
    assert(stack.pop() == 0);

    std::cout << "Stack successfully tested." << std::endl;

}
