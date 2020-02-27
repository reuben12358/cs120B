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

void set_PWM(double f) {
    static double frequency;
    if (f != frequency) {
        if(!f) TCCR3B &= 0x08;
        else TCCR3B |= 0x03;
        
        if(f < 0.954) OCR3A = 0xFFFF;
        else if (f > 31250) OCR3A = 0x0000;
        else OCR3A = (short)(8000000 / (128 * f)) - 1;

        TCNT3 = 0;
        frequency = f;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
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
unsigned long selapsed = 0;
const unsigned short tperiod = 300;
const unsigned short operiod = 1000;
const unsigned short speriod = 20;

unsigned char sp_freq = 200;

enum Speaker {sp_start, sp_on, sp_off, sp_inc, sp_dec} Speaker_state;

void sp_tick() {
	switch(Speaker_state) {
		case sp_start:
			Speaker_state = sp_on;
			break;
		case sp_off
			if ((~PINA & 0x04) == 0x04) Speaker_state = sp_on;
			if ((~PINA & 0x01) == 0x01) Speaker_state = sp_inc;
            if ((~PINA & 0x02) == 0x02) Speaker_state = sp_dec;
            break;
        case sp_on:
            if ((~PINA & 0x04) != 0x00) Speaker_state = sp_off;
            break;
		case sp_inc:
			Speaker_state = sp_off;
			break;
		case sp_dec:
			Speaker_state = sp_off;
			break;
        default: break;
	}
	
	switch(Speaker_state) {
        case sp_start:
        case sp_on: set_PWM(sp_freq);   break;
		case sp_inc: 
			if (sp_freq > 1000) sp_freq+=50;
			set_PWM(sp_freq);
			break;
		case sp_dec: 
			if (sp_freq < 0) sp_freq-=50;
			set_PWM(sp_freq);
			break;
        case sp_off: set_PWM(0);   break;
        default: break;
    }
}

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
		if (selapsed >= speriod) {
			sp_tick();
			selapsed = 0;
		}
		PORTB = tled | oled;
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

