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
#include "io.h"
#include <avr/interrupt.h>
#include <util/delay.h>

// bit.h

unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value) 
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets bit from a PINx
//Parameter: Takes in a uChar for a PINx and the pin number
//Returns: The value of the PINx
unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}


// timer.h

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

// code for lab 11

unsigned char GetKeypadKey() {
	// Check keys in col 1
	// Enable col PC4 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }
	
	// Check keys in col 2
	// Enable col PC5 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }
	
	// ... *****FINISH*****
	// Check keys in col 3
	// Enable col PC6 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }
	
	// ... *****FINISH*****
	// Check keys in col 4
	// Enable col PC7 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }
	// ... *****FINISH*****
	return('\0'); // default value
}

// task scheduler

typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

// shared variables
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

// states for pause button sm tick
enum pauseButtonSM_States { pauseButton_wait, pauseButton_press, pauseButton_release};

// pause button SM
int pauseButtonSMTick(int state){
	unsigned char press = GetKeypadKey();
	switch(state){
		case pauseButton_wait:
			state = (press == '\0')? pauseButton_wait: pauseButton_press; break;
		case pauseButton_press:
			state = pauseButton_release; break;
		case pauseButton_release:
			state = (press == '\0')? pauseButton_wait: pauseButton_release; break;
		default:
			state = pauseButton_wait;break;
	}
	switch(state){
		case pauseButton_wait:
			break;
		case pauseButton_press:
			switch(press){
				case '\0': PORTB = 0x1F;break;//All 5 LEDs on
				case '1': PORTB = 0x01; break;//hexequivalent
				case '2': PORTB =0x02; break;
				case '3': PORTB =0x03; break;
				case '4': PORTB =0x04; break;
				case '5': PORTB =0x05; break;
				case '6': PORTB =0x06; break;
				case '7': PORTB =0x07; break;
				case '8': PORTB =0x08; break;
				case '9': PORTB =0x09; break;
				case 'A': PORTB =0x0A; break;
				case 'B': PORTB =0x0B; break;
				case 'C': PORTB =0x0C; break;
				case 'D': PORTB =0x0D; break;
				case '*': PORTB =0x0E; break;
				case '0': PORTB =0x00; break;
				case '#': PORTB =0x0F; break;
				default: PORTB = 0x1B; break;//Should never occur, Middle LED off.
			}
			break;
		case pauseButton_release:
			break;
		default:
			break;
	}
	return state;
}

void main() {
	PORTA = 0x0F; DDRA = 0xF0;
	PORTB = 0x00; DDRB = 0xFF;

	unsigned long int GCD = 1;
	TimerSet(GCD);
	TimerOn();

	// declare array of tasks
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);	

	// declare tasks 1
	task1.state = pauseButton_wait;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonSMTick;
	
	unsigned short i;

	while (1) {
		// task scheduler
		for (i = 0 ; i < numTasks; i++) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]-> state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}			
			tasks[i]->elapsedTime += GCD;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return;
}

