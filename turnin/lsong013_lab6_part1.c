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
#include <avr/interrupt.h>

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

enum state {start, one, two, three} state;
void tick() {
	switch(state) {
		case start: 
			state = one;
			break;
		case one: 
			state = two;
			break;
		case two:
			state = three;
			break;
		case three: 
			state = one;
			break;
		default:
			state = start;
			break;
	}
	switch(state) {
		case start: 
			break;
		case one: 
			PORTB = 0x01;
			break;
		case two:
			PORTB = 0x02;
			break;
		case three: 
			PORTB = 0x04;
			break;
		default:
			break;
	}
	
}


void main() {
    DDRB = 0xFF;
    PORTB = 0x00;
    TimerSet(30);
    TimerOn();

    unsigned char tmpB = 0x00;
    while(1) {
        tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}


