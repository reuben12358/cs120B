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

// unsigned char C = 0x00;

enum BS_states {start, lock, temp, unlock } state;

void button_switch() {
	switch (state) {
		case start :
			state = lock;
			PORTB = 0x00;
			break;

		case lock :
			if ((PINA & 0x0F) == 0x04) {
				state = temp;
			}
			else {
				state = lock;
			}
			break;

		case temp :
			if ((PINA & 0x0F) == 0x04) {
				state = temp;
			}
			else if ((PINA & 0x0F) == 0x02) {
				PORTB = 0x01;
				state = unlock;
			}
			else {
				state = lock;
			}
			break;

		case unlock :
			if ((PINA & 0xF0) == 0x80) {
				PORTB = 0x00;
				state = lock;
			}
			else {
				state = unlock;
			}
			break;

		default:
			state = start;
			break;

		break;	
	}
	switch(state) {   // State actions
		case start :
		case lock :
		case temp :
		case unlock :
		default :
		break;
   	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s

	state = start;

	// unsigned char A = 0x00; // temp variable for value of A
	// C = 0x07;
	
	while(1) {
		// 1) Read input
		// pa0-3 is the input

//		A = PINA;
	
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
	
		button_switch();

		// convert cntint to binary 
		// 3) Write output
	}
	return 0;
}


