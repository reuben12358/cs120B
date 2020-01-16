/*	Author: lsong013
 *  Partner(s) Name: Reuben D'cunha
 *	Lab Section:22
 *	Assignment: Lab #2 Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned int_to_int(unsigned k)
{
    if (k == 0)
        return 0;
    if (k == 1)
        return 1; /* optional */
    return (k % 2) + 10 * int_to_int(k / 2);
}

int main(void)
{
    DDRA = 0x00;
    PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF;
    PORTB = 0x00;                  // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpB = 0x00;     // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00;     // Temporary variable to hold the value of A
    unsigned char tmpC = 0x00;     // Temporary variable to hold the value of A
    unsigned char tmpD = 0x00;     // Temporary variable to hold the value of A
    unsigned char cntavail = 0x00; // number of ports available
    int cntint = 0x00;
    while (1)
    {

        // 1) Read input
        tmpA = PINA & 0x01;
        tmpB = PINA & 0x02;
        tmpC = PINA & 0x04;
        tmpD = PINA & 0x08;

        // 2) Perform computation
        // if PA0 is 1, set PB1PB0 = 01, else = 10
        if (tmpA == 0x00)
        {
            cntint++;
            // increment cntavail
        }
        if (tmpB == 0x00)
        {
            cntint++;
            // increment cntavail
        }

        if (tmpC == 0x00)
        {
            cntint++;
            // increment cntavail
        }

        if (tmpD == 0x00)
        {
            cntint++;
            // increment cntavail
        }

        // convert cntint to binary
        // 3) Write output
        PORTB = cntint;
        if (cntint == 0x00)
        {
            PORTB = PORTB | 0x80;
        }

        cntint = 0x00;
    }
    return 0;
}
