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
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of fuel	
	unsigned char A = 0x00; // temp variable for A's weight
	unsigned char B = 0x00; // temp variable for B's weight
	unsigned char C = 0x00; // temp variable for C's weight

	while(1) {
		// 1) Read input
		// pa0-3 is the input

		A = PINA;
		B = PINB;
		C = PINC;

		tmpA = A+B+C;

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
		
		if (tmp > 0x8C) {
			tmpA = tmpA | 0x01;
		}
		else {
			tmpA = tmpA & 0xFE;
		}

		if ((A > C + 0x50) | (C > A + 0x50)) {
			tmpA = tmpA | 0x02;
		}
		else {
			tmpA = tmpA & 0xFD;
		}

		// convert cntint to binary 
		// 3) Write output
		PORTD = tmpA;
	
		tmpA = 0x00; // Reset value else keeps incrementing
	}
	return 0;
}


