/**************************************************************************** 
 
                                                        Kyle Sit 
                                                        20 October 2015 
                                Calculator Using Stack 
 
File Name:      calc.c 
Description:    This program contains methods that manipulate stacks to 
		emulate a calculator.
****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define BYTE 8
#define SIGN_BIT (1L << 63)

static char operators[] = "()+-*/^ !";

static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*-------------------------------------------------------------------------- 
Function Name:         eval
Purpose:               This method takes the stack from intopost and evaluates
		       the function initially inputted. 
Description:           First we reverse stack 1 onto stack 2. Then we move 
		       digits back to stack 1, taking operators from stack 2
		       and performing that operator function on the values in 
		       stack 1. The operator functions follow precdence.
Input:                 @param stack1 is a pointer to the stack containing the 
		       expression in postfix order.
Result:                We should return the final evaluation of the initial
		       expression.
--------------------------------------------------------------------------*/
long eval (Stack * stack1) {
	
	/* Allocate local stack 2 and initialize long holders and result */
	Stack* stack2 = new_Stack(sizeof(stack1));
	long holder = NULL;
	long holder2 = NULL;
	long holder3 = NULL;
	long result = NULL;

	/* Loop reversing stack1 onto stack2 */
	while( !isempty_Stack(stack1) ) {
	
		/* pop holder from stack1 and push onto stack2 */
		pop( stack1, &holder );
		push( stack2, holder );
	
	}
	
	/* Loop performing operator functions */
	while( !isempty_Stack(stack2) ) {
	
		/* Initial pop */
		pop( stack2, &holder );

		/* Check for digit */
		if( holder >= 0 ) {
			
			/* Push the digits onto stack1 */
			push( stack1, holder );

		}
		
		/* Check for nondigit */
		if( holder < 0 ) {
		
			/* Pop stack1 digits to perform operator function */
			pop( stack1, &holder2 );
			pop( stack1, &holder3 );
			
			/* Find operator in functions array perform function */
			holder3 = functions[INDEX(holder)](holder2,holder3);
			
			/* Push that final value back onto stack 1 */
			push( stack1, holder3 );

		}
		
	}
	
	/* Deallocate memory */
	delete_Stack( &stack2 );

	/* Pop final result and return it */
	pop( stack1, &result );
	return result;
}

/*-------------------------------------------------------------------------- 
Function Name:         intopost
Purpose:               This method uses a stack to convert an expression from
		       infix to postfix.
Description:           Using 2 stacks and precedence, we move digits onto stack
		       1 and operators onto stack 1 from stack 2 according to
		       precedence.  The paranthesis also take precedence.
Input:                 @param stack1 is a pointer to the stack we want to 
                       store our postfix expression into.
Result:                We should end up with a stack in postfix expression.
		       We return 1 indicating success.
--------------------------------------------------------------------------*/
long intopost (Stack * stack1) {

	/* Initialize local stack and character to store values */
	Stack* stack2 = new_Stack(sizeof(stack1));
	long character = fgetc(stdin);
	long decdig = NULL;
	long nondig = NULL;

	while( character != '\n' ) {
		
		/* Check for EOF */
		if( character == EOF ) {
			
			delete_Stack( &stack2 );
			return EOF;
		}
		
		/* Checks for space */
		if( isspace( character ) ) {
		
			character = fgetc(stdin);
			continue;
		
		}

		/* Check to see if character is a digit */
		if( character >= '0' && character <= '9' ) {

			/* Put character back into buffer for decin */
			ungetc( character, stdin );
			
			/* Call decin to get decimal number */
			decdig = decin();

			/* Push the decimal number onto the stack */
			push( stack1, decdig );

		}

		/* Check for '(' */
		else if( character == '(' ) {
		
			/* Push the '(' onto stack 2 */
			push( stack2, character );

		}

		/* Check for ')' */
		else if( character == ')' ) {
		
			if( isempty_Stack( stack2 ) ){
			
				push( stack2, character );

			}

			/* Pop the first item on stack 2 */
			pop( stack2, &nondig );
			
			/* Push the item onto stack 1 and then continue to pop 
			 * from stack 2 and push to 1 until we reach the ')' */
			while( nondig != '(' ) {
		
				push( stack1, nondig );
				pop( stack2, &nondig );

			}

		}

		else {
		
			/* Top the value of stack2 to compare */
			top( stack2, &nondig );
			
			/* Transforming character to operator */
			character = setupword(character);
			
			/* If stack 2 is empty we push onto it */
			if( isempty_Stack( stack2 ) ) {
			
				push( stack2, character );

			}

			/* If the operator's priority is higher than the last 
			 * one on stack 2 then we push */
			else if( PRIORITY( character ) > 
				PRIORITY( nondig ) ) {

				push( stack2, character );

			}

			/* If the operator's priority is lower than the last
			 * one on stack 2 then we pop stack 2 and push to
			 * stack 1 until the operator's priority is higher
			 * than the last one on stack 2 */
			else {
			
				while( PRIORITY( character ) 
					< PRIORITY( nondig ) ) {
				
					/* Pop nondig from stack 2 then push 
					 * onto stack 1. Set nondig to next 
					 * value on top of stack 2 for next
					 * loop */
					pop( stack2, &nondig );
					push( stack1, nondig );
					top( stack2, &nondig );
				
				}

				/* Push character onto stack 2 afterward */
				push( stack2, character );

			}
		
		}

		character = fgetc(stdin);

	}
	
	/* Loop pushing the rest of stack2 onto stack1 */
	while( !isempty_Stack( stack2 ) ) {
	
		push( stack1, pop( stack2, &decdig ) );
		
	}

	/* Deallocate memory and successful return */
	delete_Stack( &stack2 );
	return 1;

}

static long add (long augend, long addend) {
    return augend + addend;
}

static long divide (long divisor, long dividend) {
    return dividend / divisor;
}

/*-------------------------------------------------------------------------- 
Function Name:         exponent
Purpose:               This method emulates the exponent math function. 
Description:           Using a for loop we multiply the base by itself according
		       to the indicated power.
Input:                 @param power is the indicated power being raised to
                       @param base is the indicated number we want to raise
		       to power.
Result:                Returns base ^ power
--------------------------------------------------------------------------*/
static long exponent (long power, long base) {
    
     /* Initialize index and basehold for the loop */
     long index;
     long basehold = base;

     /* Loop multiplying base by itself power times */
     for( index = 1; index < power; index++ ) {
	
	basehold = basehold * base;
     
     }

     /* Return result */
     return basehold;
}

/*-------------------------------------------------------------------------- 
Function Name:         fact
Purpose:               This method emulates a factorial
Description:           Using a for loop we multiply a number by the successive
		       values beneath it.
Input:                 @param xxx is the number we want to perform the factorial
		       function on
                       @param ignored is unused
Result:                Returns xxx!
--------------------------------------------------------------------------*/
static long fact (long xxx, long ignored) {
     
     /* Initialize index and xxxhold for the loop */
     long index;
     long xxxhold = xxx;

     /* Loop through multiplying by one less than previous value */
     for( index = xxx - 1; index > 0; index-- ){
	
	xxx = xxx * (--xxxhold);
     
     }
     
     /* Return final factorial result */
     return xxx;
}

static long mult (long factorx, long factory) {
    return factorx * factory;
}

/*-------------------------------------------------------------------------- 
Function Name:         setupword
Purpose:               This method transforms character ascii value to operator 
Description:           Using bitwise ors we create a binary code containing
		       a sign bit, index, and character value.
Input:                 @param character is the operator function
Result:                A number that can be distinguished as an operator.
--------------------------------------------------------------------------*/
static long setupword (int character) {
	
	/* Initialize index to find spot of operator in array */
	long index = 0;

	/* Loop to increment index to right spot */ 
	while( operators[index] != character ) {
	
		index++;
	
	}
	
	/* Return distinction, index, and character value using bitwise or */
	return SIGN_BIT | (index<<8) | character;
}

static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

