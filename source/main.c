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

// REFERENCE http://alumni.cs.ucr.edu/~hdomi001/breakout.pdf

unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value) 
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}

unsigned short Controller_Keys = 0;

#define clock 0
#define Latch 1
#define data  2
#define HIGH  1
#define LOW  0

unsigned char ARRAY[2] = {LOW, HIGH};
unsigned char clock_cnt = 1;
unsigned char latch_cnt = 0;

#define NONE            0
#define SNES_B        32768  
#define SNES_Y        16384  
#define SNES_SELECT   8192   
#define SNES_START    4096   
#define SNES_UP       2048   
#define SNES_DOWN     1024   
#define SNES_LEFT      512   
#define SNES_RIGHT     256   
#define SNES_A         128   
#define SNES_X          64   
#define SNES_L          32   
#define SNES_R          16   

void Controller()
{
	// clock = 0
	// Latch = 1
	// data = 2
	
	Controller_Keys = 0;
	PORTA = SetBit(PORTA,Latch,ARRAY[latch_cnt]);
	PORTA = SetBit(PORTA,clock,ARRAY[clock_cnt]);
	
	latch_cnt = 1;
	PORTA = SetBit(PORTA,Latch,ARRAY[latch_cnt]);
	latch_cnt = 0;
	PORTA = SetBit(PORTA,Latch,ARRAY[latch_cnt]);
	
	Controller_Keys = GetBit(~PINA, data);
	
	unsigned char i;
	for (i = 0; i < 14; i++) 
	{
		clock_cnt = 0;
		PORTA = SetBit(PORTA,clock,ARRAY[clock_cnt]);
		
		Controller_Keys <<= 1;
		Controller_Keys = Controller_Keys + GetBit(~PINA, data);
		
		clock_cnt = 1;
		PORTA = SetBit(PORTA,clock,ARRAY[clock_cnt]);
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
		
	while(1) {
		Controller();
	    if (Controller_Keys & SNES_Y) {
			PORTB = 0xff;
		}
		else {
			PORTB = 0x00;
		}
	}
	return 0;
}
