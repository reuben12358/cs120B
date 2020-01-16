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


unsigned numberOnes(unsigned num) {
	int i;
	int ones=0x00;
	for(i = 0; i < 8; i++) {
		if(num & 1) 
			ones++;

		num >>= 1;
	}

	return ones;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as outputs, initialize to 0s
DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	//unsigned char tmpA = 0x00; // Temporary variable to hold the value of A0	
	
	while(1) {
		// 1) Read input
		int aOnes = numberOnes(PINA);
		int bOnes = numberOnes(PINB);

		int totalOnes = 0x00;
		totalOnes += aOnes + bOnes;
		
		// 2) Perform computation
		// if PA0 is 1, set PB1PB0 = 01, else = 10
		
		// convert cntint to binary 
		// 3) Write output
		PORTC = totalOnes;
	
		totalOnes = 0x00; // Reset value else keeps incrementing
	}
	return 0;
}


