#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

const char INFIX_EXPRESSION[] = "\nPlease enter an expression to calculate:  ";
const char POSTFIX_EXPRESSION[] = "\nThe expression in postfix order is:  ";
const char RESULT[] = "\n\twhich evaluates to:  ";

int main (int argc, char * const * argv) {
        char option;
        long intopost_status;   /* return value from intopost */
        Stack * main_Stack;

        /* initialize debug states */
        debug_off ();

        /* check for stack debug options */
        while ((option = getopt (argc, argv, "x")) != EOF) {
                switch (option) {
                        case 'x': debug_on (); break;
                }
        }

        main_Stack = new_Stack (CALCSTACKSIZE); /* for the postfix expression */

        while (1) {
                writeline (INFIX_EXPRESSION, stdout);
                if ((intopost_status = intopost (main_Stack)) == EOF)
                        break;
                
                if (intopost_status == FALSE) {
                        empty_Stack (main_Stack);
                        continue;
                }

                writeline (POSTFIX_EXPRESSION, stdout);
                write_Stack (main_Stack, stdout);

                writeline (RESULT, stdout);
                decout (eval (main_Stack));
                newline ();
        }

        delete_Stack (&main_Stack);
        newline ();
        return 0;
}
