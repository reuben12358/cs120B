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

enum BS_states {BS_tempstart, BS_start, BS_plus, BS_minus, BS_zero } BS_state;

void button_switch() {
	switch (BS_state) {
		case BS_tempstart :
			BS_state = BS_start;
			PORTC = 0x07;
			break;

		case BS_start :
			if (PINA & 0x01) {
				if (PINA & 0x03) {
					BS_state = BS_zero;
					PORTC = 0;
				}
				else {
					BS_state = BS_plus;
					if (PORTC < 9) ++PORTC;
				}
			}
			else if (PINA & 0x02) {
				if (PINA & 0x03) {
					BS_state = BS_zero;
					PORTC = 0;
				}
				else {
					BS_state = BS_minus;
					if (PORTC > 0) --PORTC;
				}			
			}
			else {
				PORTC = PORTC;
			}
			break;

		case BS_plus :
			if (PINA & 0x01) {
				BS_state = BS_plus;
			}
			else if (!(PINA & 0x0F)) {
				BS_state = BS_start;
			}
			else if (PINA & 0x03) {
				BS_state = BS_zero;
				PORTC = 0;
			}
			else {
				PORTC = PORTC;
			}
			break;

		case BS_minus :
			if (PINA & 0x02) {
				BS_state = BS_minus;
			}
			else if (!(PINA & 0x0F)) {
				BS_state = BS_start;
			}
			else if (PINA & 0x03) {
				BS_state = BS_zero;
				PORTC = 0;
			}
			else {
				PORTC = PORTC;
			}
			break;

		case BS_zero :
			BS_state = BS_start;
			PORTC = PORTC;
			break;

		default:
			PORTC = 0x07;
			BS_state = BS_start;
			break;

		break;	
	}
	switch(BS_state) {   // State actions
		case BS_start :
		case BS_plus :
		case BS_minus :
		case BS_zero :
		default :
		break;
   	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s

	BS_state = BS_tempstart;

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


