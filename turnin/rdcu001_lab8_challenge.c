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

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
	// DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	// DDRD = 0xFF; PORTD = 0x00;
	unsigned short x;
	unsigned char b;
	unsigned char d;
	
	ADC_init();
	
	while(1) {
		x = ADC;
		
		if (x > 63)
			b = 63;
		else if ((x <= 63) && (x > 56))	
			b = 55;
		else if ((x <= 55) && (x > 48))	
			b = 47;
		else if ((x <= 47) && (x > 40))	
			b = 39;
		else if ((x <= 39) && (x > 32))	
			b = 31;
		else if ((x <= 31) && (x > 24))	
			b = 23;
		else if ((x <= 23) && (x > 16))	
			b = 15;
		else if ((x <= 15) && (x > 8))	
			b = 7;
		else
			b = 0;
		
		// b = (char)x;
		// d = (char)(x >> 8); 
		PORTB = b;
		// PORTD = d;
		// 1) Read input
		// 2) Perform computation
		// 3) Write output
	}
	return 0;
}


