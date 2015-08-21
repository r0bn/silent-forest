#include "StackCounter.h"

void StackCounter::count_elements()
{
    // empty counter
    for(int i = 0; i < MAX_ELEMENTS_COUNTABLE;i++)
    {
        StackCounter::counter[i] = 0;
    }

    for(int i=0;i < StackCounter::pointer;i++) 
    {
        int element = StackCounter::elements[i];
        StackCounter::counter[element]++;
    }
}

int StackCounter::top_occurence()
{
    int temp = -1;
    int pointerMax = -1;
    for(int i=0;i < MAX_ELEMENTS_COUNTABLE;i++)
    {
        //ignore zeros
        if(StackCounter::counter[i] == 0)
        {
            continue;
        }

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
