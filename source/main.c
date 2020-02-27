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

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	// DDRD = 0xFF; PORTD = 0x00;
		
	while(1) {
	
		if (PINA) {
			PORTB = 0xff;
		}	
		// b = (char)x;
		// d = (char)(x >> 8);
		// PORTD = d;
		// 1) Read input
		// 2) Perform computation
		// 3) Write output
	}
	return 0;
}


