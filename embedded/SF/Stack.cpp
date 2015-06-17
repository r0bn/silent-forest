#include "Stack.h"

int Stack::top()
{
    if(Stack::pointer > 0)
    {
        return Stack::elements[Stack::pointer - 1];
    }
    else
    {
        return 0;
    }
}

void Stack::push(int element)
{
    if(Stack::pointer < MAX_ELEMENTS) 
    {
        Stack::elements[Stack::pointer] = element;
        Stack::pointer += 1; 
    }
}

int Stack::pop()
{
    int ret = 0;

    if(Stack::pointer > 0)
    {
        Stack::pointer -= 1;
        ret = Stack::elements[Stack::pointer];
    }

    return ret;
}

int Stack::count()
{
    return Stack::pointer;
}

void Stack::empty()
{
    Stack::pointer = 0;
}
