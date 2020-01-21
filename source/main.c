/*	Author: lsong013
 *  Partner(s) Name: Reuben D'cunha
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

enum BS_states {BS_First, BS_start, BS_plus, BS_minus, BS_zero } BS_states;

unsigned char button_switch(unsigned char A, unsigned char C) {
	switch (BS_states) {
		case BS_First :
			BS_states = BS_start;
			break;
		
		case BS_start :
			if (A & 0x01) {
				if (A & 0x03) {
					BS_states = BS_zero;
				}
				else {
					BS_states = BS_plus;
				}
			}
			else if (A & 0x02) {
				if (A & 0x03) {
					BS_states = BS_zero;
				}
				else {
					BS_states = BS_minus;
				}			
			}
			break;

		case BS_plus :
			if ((A & 0x02) | (A & 0x03)) {
				BS_states = BS_zero;
			}
			else if (!(A & 0x02)) {
				BS_states = BS_start;
			}
			break;

		case BS_minus : 
			if ((A & 0x01) | (A & 0x03)) {
				BS_states = BS_zero;
			}
			else if (!(A & 0x01)) {
				BS_states = BS_start;
			}
			break;

		case BS_zero :
			BS_states = BS_start;
			break;

		default:
			BS_states = BS_start;
			break;	
	}
	switch(BS_states) {   // State actions
    	case BS_First :
			C = 0x07;
			break;

		case BS_start :
        	break;

    	case BS_plus:
        	if (C++ < 10) {
				C++;
			}
        	break;

    	case BS_minus:
        	if (C-- > -1) {
				C--;
			}
        	break;

    	case BS_zero:
        	C = 0x00;
        	break;

	    default:
			C = 0x07;
    	    break;
   	}

	return C; 
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s

	unsigned char A = 0x00; // temp variable for value of A
	unsigned char C = 0x07; // temp variable for value of B
	
	while(1) {
		// 1) Read input
		// pa0-3 is the input

		A = PINA;
	
		// unsigned char a0 = PINA & 0x01;
		// unsigned char a1 = PINA & 0x02;
		// unsigned char a2 = PINA & 0x04;
		// unsigned char a3 = PINA & 0x08;
		// if (PA0 || PA1) {		
		// // light up pc5
		// 	tmpA = 0x20;
		// 	printf("executed");
		// }
		
		// 2) Perform computation
		// if PA0 is 1, set PB1PB0 = 01, else = 10
	
		C = button_switch(A, 7);

		// convert cntint to binary 
		// 3) Write output
		PORTC = C;
	}
	return 0;
}


