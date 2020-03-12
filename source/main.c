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
#include <util/delay.h>
#include "nokia5110.h"

// REFERENCE: melanie for snes controller code

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

unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}
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

unsigned short button = 0x0000;
int ingametimer = 0;
int numturns = 0;
int maxturns = 0;
int showtimer = 0;

enum snes {gamestart, snes_wait, snes_start, snes_select , snes_a, snes_b, snes_x, snes_y, gameover } snes_state;

void snes_tick(int state) {
	switch(state) {
		case gamestart:
			break;
		case snes_wait:
			if ((ingametimer >= 60) || (numturns >= maxturns)) {
				snes_state = gameover;
				break;
			}

			button = GetSNESIn();
			mapPlayerInput(button, ins);

			if (inputContains(ins, Start))
				snes_state = snes_start;
			else if (inputContains(ins, Select))
				snes_state = snes_select;
			else if (inputContains(ins, A))
				snes_state = snes_a;
			else if (inputContains(ins, B))
				snes_state = snes_b;
			else if (inputContains(ins, X))
				snes_state = snes_x;
			else if (inputContains(ins, Y))
				snes_state = snes_y;
			else 
				snes_state = snes_wait;
			break;
		case snes_start: // reset difficulty
			snes_state = gamestart;
			break;
		case snes_select: // reset turns
			snes_state = snes_wait;
			break;		
		case snes_a: // pour a -> b
			snes_state = snes_wait;
			break;		
		case snes_b: // empty a/b
			snes_state = snes_wait;
			break;		
		case snes_x: // fill a/b
			snes_state = snes_wait;
			break;		
		case snes_y: // show timer
			snes_state = snes_wait;
			break;	
		case gameover:
			snes_state = gamestart;
		default:
			snes_state = gamestart;
			break;
	}
	switch(state) {
		case gamestart:
			nokia_lcd_clear();
			nokia_lcd_write_string("Welcome!",2);
			nokia_lcd_write_string("Please choose Difficulty:   E(a),M(b),H(x)",1);

			button = GetSNESIn();
			mapPlayerInput(button, ins);

			if (inputContains(ins, A)) {
				maxturns = 999;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, B)) {
				maxturns = 20;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, X)) {
				maxturns = 10;
				snes_state = snes_wait;
			}
			else 
				snes_state = gamestart;
			
			ingametimer = 0;
			numturns = 0;
			break;
		case snes_wait:
			nokia_lcd_clear();
			if (showtimer) {
				nokia_lcd_write_char((char)ingametimer/10 + 0x30,3);
				nokia_lcd_write_char((char)ingametimer%10 + 0x30,3);
			}
			ingametimer++;
			break;
		case snes_start:
			ingametimer = 0;
			numturns = 0;
			
			
			break;
		case snes_select:
			numturns = 0;
			ingametimer = 0;
			break;
		case snes_a:
			numturns++;
			ingametimer = 0;
			break;
		case snes_b:
			numturns++;
			ingametimer = 0;
			break;
		case snes_x:
			numturns++;
			ingametimer = 0;
			break;
		case snes_y:
			if (!showtimer)
				showtimer++;
			else
				showtimer--;
			break;
		case gameover:
			nokia_lcd_clear();
			nokia_lcd_write_string("Gameover",2);
			nokia_lcd_write_string("Thank you for playing! PressSELECT 2 reset",1);

			button = GetSNESIn();
			mapPlayerInput(button, ins);

			if (inputContains(ins, Select)) 
				snes_state = gamestart;
			else 
				snes_state = gameover;

			ingametimer = 0;
			numturns = 0;
			maxturns = 0;
			break;
	}
}

int main(void) {
	DDRA = 0xFE; PORTA = 0x01;
	DDRB = 0xFF; PORTB = 0x00;

	// unsigned char output = 0x00;

	nokia_lcd_init();
	nokia_lcd_clear();
	
	snes_state = gamestart;
	
	while(1) {		
		// screensize is 14
		snes_tick(snes_state);
		nokia_lcd_render();
	}
	return 0;
}
