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
static unsigned char tled = 0;
static unsigned char oled = 0;

enum tstates {start, zero, one, two} tstates;
void ttick() {
	switch(tstates) {
		case start:
			tstates = zero;
			break;
		case zero:
			tstates = one;
			break;
		case one:
			tstates = two;
			break;
		case two: 
			tstates = zero;
			break;
		default:
			tstates = start;
			break;
	}

	switch(tstates) {
		case start:
			break;
		case zero:
			tled = 0x01;
			break;
		case one:
			tled = 0x02;
			break;
		case two: 
			tled = 0x04;
			break;
		default:
			break;
	}
}
		
enum ostates {ostart, three, off} ostates;

void otick() {
	switch(ostates) {
		case ostart:
			ostates = three;
			break;
		case three:
			ostates = off;
			break;
		case off:
			ostates = three;
			break;
		default:
			break;
	}

	switch(ostates) {
		case ostart:
			break;
		case three:
			oled = 0x08;
			break;
		case off:
			oled = 0x00;
			break;
		default:
			break;
	}
}

unsigned long telapsed = 0;
unsigned long oelapsed = 0;
const unsigned short tperiod = 300;
const unsigned short operiod = 1000;


void main() {
	PORTA = 0xFF; DDRA = 0x00;
	PORTB = 0x00; DDRB = 0xFF;
	TimerSet(100);
	TimerOn();
	tstates = start;

	while(1) {
		telapsed += 100;
		oelapsed += 100;
		if (telapsed >= tperiod) { 
			ttick();
			telapsed = 0;
		}
		if (oelapsed >= operiod) {
			otick();
			oelapsed = 0;
		}
		PORTB = tled | oled;
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

