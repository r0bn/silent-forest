/*
 * Stack
 * Simple Stack Implementation 
 * Only Integers Supported
 */


#ifndef __STACK_H__
#define __STACK_H__

#define MAX_ELEMENTS 50

class Stack 
{
    private:

    protected:

        /*
         * Pointer to latest element
         */
        int pointer;

        /*
         * holds all elements
         */
        int elements[MAX_ELEMENTS];

    public:
        /*
         * Constructor - Initialize variables
         */
        Stack(void) :
            pointer(0),
            elements()
        {}

        /*
         * Return's the latest element without decrementing the pointer
         */
        int top();

        /*
         * Put a new element on the stack
         * If MAX_ELEMENTS reached the element will be droped
         */
        void push(int element);

        /*
         * Return's latest element and decrements the pointer
         */
        int pop();

        /*
         * Returns the current size of the Stack
         */
        int count();

        /*
         * Empty the complete Stack
         */
        void empty();
};

#endif
