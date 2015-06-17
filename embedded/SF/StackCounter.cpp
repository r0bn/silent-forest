#include "StackCounter.h"

void StackCounter::count_elements()
{
    StackCounter::counter[MAX_ELEMENTS_COUNTABLE] = {};

    for(int i=0;i < StackCounter::pointer;i++) 
    {
        int element = StackCounter::elements[i];
        StackCounter::counter[element]++;
    }
}

int StackCounter::max_amount(int topRange)
{
    int temp = -1;
    int pointerMax = -1;
    for(int i=0;i < MAX_ELEMENTS_COUNTABLE && i < topRange;i++)
    {
        // if same as last max, then cancel because draw
        if(StackCounter::counter[i] == temp)
        {
            return -1;
        }

        if(StackCounter::counter[i] > temp)
        {
            temp = counter[i];
            pointerMax = i;
        }
    }

    return pointerMax;
}
