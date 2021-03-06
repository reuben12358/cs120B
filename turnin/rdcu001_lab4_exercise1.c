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
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s

	unsigned char A = 0x00; // temp variable for value of A
	unsigned char B = 0x01; // temp variable for value of B
	
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
	
		if (A & 0x01) {
			if (B == 0x01) {
				B = 0x02;
			}
			else {
				B = 0x01;
			}
		}

		// convert cntint to binary 
		// 3) Write output
		PORTB = B;
	}
	return 0;
}


