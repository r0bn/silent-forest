/*
 * Stack
 * Used for counting logs over time
 * Only Integers Supported
 */


#ifndef __STACK_H__
#define __STACK_H__

#define MAX_ELEMENTS 50

class Stack 
{
    private:

        int pointer;

        int elements[MAX_ELEMENTS];

    protected:

    public:
        /*
         * Constructor
         */
        Stack(void)
        {}

        int top();

        void push(int element);

        int pop();

        int count();
};

#endif
