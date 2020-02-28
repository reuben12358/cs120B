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
#define F_CPU 8000000UL 
#include <util/delay.h>

// REFERENCE: http://alumni.cs.ucr.edu/~hdomi001/breakout.pdf
// REFERENCE: mgalo001@ucr.edu for snes controller code

/*
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff()
{
	TCCR1B = 0x00;
}
void TimerISR()
{
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
*/
unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value) 
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}
#define F_CPU 8000000UL 
unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}
/*
typedef struct _task {
	// Tasks should have members that include: state, period,
	// a measurement of elapsed time, and a function pointer.
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
*/
/*
unsigned short Controller_Keys = 0;

#define clock PORTA2
#define latch PORTA1
#define data  PORTA0
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
*/
typedef enum {false, true} bool;

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

#define CLOCK  2 // Output
#define LATCH  1 // Output
#define DATA   0 // Input

enum inputs {none, B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R};

unsigned short GetSNESIn()
{
	unsigned short data = 0x0000;
	
	//request data from controller
	PORTA = SetBit(PINA, LATCH, 1);
	_delay_us(12);
	PORTA = SetBit(PINA, LATCH, 0);
	_delay_us(6);

	//read data
	unsigned char i;
	for (i = 0; i < 12; i++) {
		if(!GetBit(PINA, DATA)) {
			SET_BIT(data, i);
		} else {
			CLR_BIT(data, i);
		}
		
		//tick clock and wait for next bit 
		PORTA = SetBit(PINA, CLOCK, 1);
		_delay_us(6);
		PORTA = SetBit(PINA, CLOCK, 0);
		_delay_us(6);
	}

	return data;
}
/*
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
	int i = 0;
    for(i = 0; i < 16; i++){
        port &= ~(0x01 << clock);
        snes_pressed <<= 1;
        snes_pressed |= (((~pin) & (0x01  << data)) >> data);      
		port |= (0x01 << clock);
    }
    return snes_pressed;
}
*/
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
void mapPlayerInput(unsigned short rawInput, unsigned char* ins) {
	unsigned char count = 0;
	unsigned char i;
	for(i = 0; i < 12; i++) {
		if(GET_BIT(rawInput, i)) {
			ins[count] = i + 1;
			count++;
		}
	}
	ins[count] = 0;
}

bool inputContains(unsigned char* ins, unsigned char input) {
	unsigned char i;
	for(i = 0; i < 12; i++) {
		if(ins[i] == input) {
			return true;
		} else if(ins[i] == 0) {
			return false;
		}
	}
	return false;
}

unsigned char ins[12];
// unsigned char pressed = 0;
/*
enum SNES_SM { SNES_wait, SNES_press, SNES_hold };

int SNESInputTick(int state) {
	
	//get input from controller
 	unsigned short button = GetSNESIn();
	//PORTA = (PORTA & 0x01) | 0x00;
	//PORTB = (PORTB & 0x00) | button;	//for debugging
	mapPlayerInput(button, ins);
	
	//State machine transitions
	switch (state) {
		case SNES_wait: 
			pressed = 0;
			if(button != 0) {
				state = SNES_press;
			}
			break;
		case SNES_press: 
			state = SNES_hold;	
			pressed = 1;
			break;
		case SNES_hold:
			if(!button) {
				state = SNES_wait;
			}
			break;
		default: state = SNES_wait; // default: Initial state
			break;
	}

	//State machine actions
	switch(state) {
		case SNES_wait:
			break;
		case SNES_press:	//cheat codes ;)
			PORTB=0x01;
			break;
		case SNES_hold:	
			PORTB=0x01;
			break;
		default:		
			break;
	}

	return state;
}
*/
int main(void) {
	DDRA = 0xFE; PORTA = 0x01;
	DDRB = 0xFF; PORTB = 0x00;

	// SNES_init();

	unsigned short button = 0x0000;
	// unsigned char output = 0x00;
		
	while(1) {
		// Controller();

		// output = 0x00;
		// button = SNES_Read();
		/*if ((~PINA)&0x08) {
			PORTB = 0xff;
		}
		else {
			PORTB = 0x00;
		}*/
		/*
		task task1;
		task1.state = -1; //Task initial state.
		task1.period = 50; //Task Period.
		task1.elapsedTime = 50; //Task current elapsed time.
		task1.TickFct = &SNESInputTick; //Function pointer for the tick. 

		if (button & SNES_X) {
			PORTB = 0xff;
		}
		else {
			PORTB = 0x00;
		}*/
		/*
		while(!TimerFlag);
		task1.state = task1.TickFct(task1.state);
		TimerFlag = 0;
		*/
		
		button = GetSNESIn();
		mapPlayerInput(button, ins);
		if (inputContains(ins, Start)) {
			PORTB = 0x0F;
		}
		else {
			PORTB = 0x00;
		}
	}
	return 0;
}
