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
			num++;

		ones >>= 1;
	}

	return ones;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of fuel	
	
	while(1) {
		// 1) Read input
		// pa0-3 is the input
		unsigned char a0 = PINA & 0x01;
		unsigned char a1 = PINA & 0x02;
		unsigned char a2 = PINA & 0x04;
		unsigned char a3 = PINA & 0x08;
		if (!a2&&!a3&&!a1&&a0||!a2&&!a3&&a1&&!a0) {		
		// lvl1-2
			tmpA = 0x60;
		}
		else if ((a0&&!a3&&a1&&!a2)||(!a0&&!a3&&!a1&&a2)) {		
		// lvl3-4
			tmpA = 0x70;
		}
		else if ((!a3&&a2&&!a1&&a0)||(!a3&&a2&&a1&&!a0)) {		
		// lvl5-6
			tmpA = 0x38;
		}
		else if ((!a3&&a2&&a1&&a0)||(a3&&!a2&&!a1&&!a0)||(a3&&!a2&&!a1&&a0)) {		
		// lvl7-9
			tmpA = 0x3C;
		}
		else if ((a3&&!a2&&a1&&!a0)||(a3&&!a2&&a1&&a0)||(a3&&a2&&!a1&&!a0)) {		
		// lvl10-12
			tmpA = 0x3E;
		}
		else if ((a3&&a2&&a1&&a0)||(a3&&a2&&a1&&!a0)||(a3&&!a2&&!a1&&a0)) {		
		// lvl13-15
			tmpA = 0x3F;
		}
		
		// 2) Perform computation
		// if PA0 is 1, set PB1PB0 = 01, else = 10
		
		// convert cntint to binary 
		// 3) Write output
		PORTC = tmpA;
	
		tmpA = 0x00; // Reset value else keeps incrementing
	}
	return 0;
}


