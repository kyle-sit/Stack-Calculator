#ifndef STACK_H
#define STACK_H

#include <stdio.h>

/* This array implementation of stack is an array of longs (words), the
stack count is the first element in the array, the stack size is the second
element in the array, and the stack pointer is the third element in the
array.  The stack pointer has the value of an index into the array to denote
the last used space in the stack. */

typedef long Stack;

void delete_Stack (Stack **);   /* deallocates memory allocated in new_Stack.
                                   Assigns incoming pointer to NULL. */
void empty_Stack (Stack *);     /* empties the stack */
long isempty_Stack (Stack *);   /* returns 0 or non-0 value indicating
                                   whether or not the stack is empty */
long isfull_Stack (Stack *);    /* returns 0 or non-0 value indicating
                                   whether or not the stack is full */
Stack * new_Stack (unsigned long); /* allocates stack array, and initializes
                                   stack pointer.  Result is a pointer in the
                                   array where user data allotment begins */
long num_elements (Stack *);     /* returns the number of elements stored
                                   on the stack. */
long pop (Stack *, long *);     /* removes and sends back the top element of
                                   the stack.  Result is 0 or non-0,
                                   indicating failure or success,
                                   respectively */
long push (Stack *, long);      /* places one value on the specified stack.
                                   Result is 0 or non-0 indicating failure
                                   or success, respectively */
long top (Stack *, long *);     /* sends back the top element of the stack.
                                   Stack is left unaffected. Result is 0
                                   or non-0 indicating failure or success,
                                   respectively. */
FILE * write_Stack (Stack *, FILE *); /* prints out the contents of the stack
                                   to the parameter specified FILE */

void debug_on (void);     /* turns stack debugging on */
void debug_off (void);    /* turns stack debugging off */

#endif
