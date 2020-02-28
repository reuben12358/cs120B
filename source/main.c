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

// REFERENCE: http://alumni.cs.ucr.edu/~hdomi001/breakout.pdf
// REFERENCE: mgalo001@ucr.edu for snes controller code

unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value) 
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}

unsigned short Controller_Keys = 0;

#define clock PORTA0
#define latch PORTA1
#define data  PORTA2
#define port  PORTA
#define pin   PINA

// unsigned char ARRAY[2] = {LOW, HIGH};
// unsigned char clock_cnt = 1;
// unsigned char latch_cnt = 0;

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

// #define OUTPUT_PORT PORTD

void SNES_init(){
    port |= (0x01 << clock);
    port |= (0x01 << latch);
}

unsigned short SNES_Read(){
    unsigned short snes_pressed = 0x0000;
      
    // Turn latch on and off. Send signal to SNES controller 

    port |= (0x01 << latch);
	port |= (0x01 << clock);
    port &= ~(0x01 << latch);
    
    snes_pressed = (((~pin) & (0x01 << data)) >> data);
    
    // For 16 clock cycles the controller outputs the keys pressed, 
	// but first one is a bit different and some not used.
	// See JChristy Part 6. 
	int i = 0;
    for(i = 0; i < 16; i++){
        port &= ~(0x01 << clock);
        snes_pressed <<= 1;
        snes_pressed |= (((~pin) & (0x01  << data)) >> data);      
		port |= (0x01 << clock);
    }
    return snes_pressed;
}

/*
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
*/

int main(void) {
	DDRA = 0x03; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;

	SNES_init();

	unsigned short button = 0x0000;
	// unsigned char output = 0x00;
		
	while(1) {
		// Controller();

		// output = 0x00;
		button = SNES_Read();

	    if ((button & SNES_Y) == SNES_Y) {
			PORTB = 0xff;
		}
		else {
			PORTB = 0x00;
		}
	}
	return 0;
}
