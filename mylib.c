#include <ctype.h>
#include <stdio.h>
#include "mylib.h"

#define ASCII_ZERO 0x30
#define COUNT ((long) (sizeof (long) << 1))

static char hexdigits[] = "0123456789ABCDEF";

static void posdecout (unsigned long number);

void clrbuf (int character) {

        /* check for buffer already being empty */
        /* then remove all characters from buffer until empty */
        while (character != '\n')
                character = getchar ();
}

long decin (void) {
        int character;          /* ASCII input from the user */
        long digit;             /* each digit as it is found */
        long sum = 0;           /* accumulated number */

        /* input terminates with a non-digit character */
        while (isdigit (character = getchar())) {

                /* change from ASCII */
                digit = character & ~ASCII_ZERO;

                /* accumulate number */
                sum *= 10;
                sum += digit;
        }

        /* return non-digit to input stream */
        ungetc (character, stdin);

        /* result found */
        return sum;
}

void decout (long number) {
        if (number < 0) {
                putchar ('-');
                posdecout (- number);
        }
        else
                posdecout (number);
}

void hexout (unsigned long number) {
        unsigned long count = 0;        /* counter for loop */
        unsigned long digit;            /* individual digit for output */

        /* output "0x" for hexidecimal */
        writeline ("0x", stdout);

        for (; count < COUNT; count++) {

                /* get digit to print */
                digit = number >> ((COUNT - 1) << 2);

                /* output digit in ASCII*/
                putchar (hexdigits[digit]);

                /* set up for next digit */
                number <<= 4;
        }
}

void newline (void) {
        putchar ('\n');
}

long writeline (const char * string, FILE * stream) {
        long index = 0;         /* generic loop index */

        /* output string character by character */
        while (string[index])
                fputc(string[index++], stream);

        /* return the length of string */
        return index;
}

static void posdecout (unsigned long number) {
        unsigned long quotient;         /* quotient of div by 10 */
        long remainder;         /* remainder of div by 10 */

        /* remove one digit from number
                digit (remainder) is for output, 
                the rest of the number (quotient) still needs to be printed */
        remainder = number % 10;
        quotient = number / 10;

        /* call function to output remaining digits, if any */
        if (quotient)
                posdecout (quotient);

        /* output digit */
        putchar (hexdigits [remainder]);
}
