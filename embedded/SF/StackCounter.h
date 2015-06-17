/*
 * StackCounter
 * Used for counting logs over time
 * inherted from Stack
 * Only Integers Supported
 */
#include "Stack.h"

#ifndef __STACKCOUNTER_H__
#define __STACKCOUNTER_H__

// How much different elements are recognized, starts from zero
#define MAX_ELEMENTS_COUNTABLE 10

class StackCounter : public Stack
{
    private:

    protected:

    public:
        /*
         * Constructor - Initialize variables
         */
        StackCounter(void) : 
            counter()
        {}

        /*
         * hold the return for counting elements
         */
        int counter[MAX_ELEMENTS_COUNTABLE];

        /*
         * Count's every element in the Stack
         * without to empty the stack
         *
         * No Return, the return is saved in the class variable counter
         */
        void count_elements();

        /*
         * Return's the element with the max appeareance in the Stack
         * without to empty the stack
         * Important: based on the previous call from count_elements
         *
         * topRange: define's the range that should be compared
         *
         * Return's "-1" if there even max amounts
         */
        int max_amount(int topRange);

};

#endif
