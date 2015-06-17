#include "Stack.h"

int Stack::top()
{
    return Stack::elements[Stack::pointer];
}

void Stack::push(int element)
{
    if((Stack::pointer + 1) < MAX_ELEMENTS) 
    {
        Stack::pointer += 1; 
        Stack::elements[Stack::pointer] = element;
    }
}

int Stack::pop()
{
    int ret = 0;

    if(Stack::pointer > 0)
    {
        ret = Stack::elements[Stack::pointer];
        Stack::pointer -= 1;
    }

    return ret;
}

int Stack::count()
{
    return Stack::pointer;
}
