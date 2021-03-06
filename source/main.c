/*	Author: Reuben D'cunha
 *	Lab Section: 22
 *	Assignment: Custom Lab Project
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
#include <stdlib.h>

// REFERENCE: melanie for snes controller code

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
int capA = 0;
int capB = 0;
int maxA = 0;
int maxB = 0;
int target = 0;
int gamewin = 0;

void randomizer() {
	int r = rand();
	
	if (r%5 == 0) {
		maxA = 8;
		maxB = 2;
		target = 4;
	}
	else if (r%5 == 1) {
		maxA = 5;
		maxB = 2;
		target = 1;
	}
	else if (r%5 == 2) {
		maxA = 7;
		maxB = 2;
		target = 3;
	}
	else if (r%5 == 3) {
		maxA = 4;
		maxB = 1;
		target = 3;
	}
	else {
		maxA = 5;
		maxB = 3;
		target = 4;
	}
}

enum snes {gamestart, snes_wait, snes_start, snes_select , snes_a, snes_b, snes_x, snes_y, gameover, choice_a, choice_b, choice_x } snes_state;

void snes_tick(int state) {
	switch(state) {
		case gamestart:
			break;
		case snes_wait:
			if ((ingametimer >= 60) || (numturns >= maxturns)) {
				snes_state = gameover;
				break;
			}
			if ((capA == target) || (capB == target)) {
				gamewin = 1;
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
			snes_state = choice_a;
			break;		
		case choice_a: // choose a/b to pour into b/a
			if (ingametimer >= 60) {
				snes_state = gameover;
				break;
			}
			button = GetSNESIn();
			mapPlayerInput(button, ins);
			if (inputContains(ins, A)) {
				while ((capA < maxA) && (capB > 0)) {
					capA++;
					capB--;
				}
				ingametimer = 0;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, B)) {
				while ((capA < maxA) && (capB > 0)) {
					capA++;
					capB--;
				}
				while ((capB < maxB) && (capA > 0)) {
					capA--;
					capB++;
				}
				ingametimer = 0;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, X)) {
				snes_state = snes_wait;
				break;
			}
			else {
				snes_state = choice_a;
			}
			break;
		case snes_b: // empty a/b
			snes_state = choice_b;
			break;		
		case choice_b: // choose a,b to empty
			if (ingametimer >= 60) {
				snes_state = gameover;
				break;
			}
			button = GetSNESIn();
			mapPlayerInput(button, ins);
			if (inputContains(ins, A)) {
				capA = 0;
				ingametimer = 0;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, B)) {
				capB = 0;
				ingametimer = 0;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, X)) {
				snes_state = snes_wait;
				break;
			}
			else {
				snes_state = choice_b;
			}
			break;
		case snes_x: // fill a/b
			snes_state = choice_x;
			break;		
		case choice_x: // choose a,b to fill
			if (ingametimer >= 60) {
				snes_state = gameover;
				break;
			}
			button = GetSNESIn();
			mapPlayerInput(button, ins);
			if (inputContains(ins, A)) {
				capA = maxA;
				ingametimer = 0;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, B)) {
				capB = maxB;
				ingametimer = 0;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, X)) {
				snes_state = snes_wait;
				break;
			}
			else {
				snes_state = choice_x;
			}
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
				maxturns = 80;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, B)) {
				maxturns = 40;
				snes_state = snes_wait;
			}
			else if (inputContains(ins, X)) {
				maxturns = 20;
				snes_state = snes_wait;
			}
			else {
				snes_state = gamestart;
			}
			gamewin = 0;
			capA = 0;
			capB = 0;
			randomizer();
			ingametimer = 0;
			numturns = 0;
			break;
		case snes_wait:
			nokia_lcd_clear();
			nokia_lcd_write_string("Jug A = ",1);
			nokia_lcd_write_char((char)capA + 0x30,1);
			nokia_lcd_write_char(0x2f,1);
			nokia_lcd_write_char((char)maxA + 0x30,1);
			nokia_lcd_write_string("   Jug B = ",1);			
			nokia_lcd_write_char((char)capB + 0x30,1);
			nokia_lcd_write_char(0x2f,1);
			nokia_lcd_write_char((char)maxB + 0x30,1);
			
			nokia_lcd_write_string("   Target = ",1);
			nokia_lcd_write_char((char)target + 0x30,1);			
			
			if (!showtimer) {
				nokia_lcd_write_string("    Timer = ",1);
				nokia_lcd_write_char((char)ingametimer/10 + 0x30,1);
				nokia_lcd_write_char((char)ingametimer%10 + 0x30,1);
			}
			else {
				nokia_lcd_write_string("              ",1);
			}
			
			nokia_lcd_write_string("    Select Action A,B,X,Y,St,Se",1);
			ingametimer++;
			break;
		case snes_start:
			ingametimer = 0;
			numturns = 0;
			maxturns = 0;
			capA = 0;
			capB = 0;
			maxA = 0;
			maxB = 0;
			break;
		case snes_select:
			numturns = 0;
			ingametimer = 0;
			capA = 0;
			capB = 0;
			break;
		case snes_a: // pour a -> b
			numturns++;
			ingametimer = 0;
			break;
		case choice_a:
			nokia_lcd_clear();
			nokia_lcd_write_string("choose jug to pour into A/B ", 1);
			if (!showtimer) {
				nokia_lcd_write_string("Timer = ",1);
				nokia_lcd_write_char((char)ingametimer/10 + 0x30,1);
				nokia_lcd_write_char((char)ingametimer%10 + 0x30,1);
			}
			else {
				nokia_lcd_write_string("              ",1);
			}
			nokia_lcd_write_string("                  ",1);
			nokia_lcd_write_string("X to go back",1);
			
			ingametimer++;
			break;
		case snes_b: // empty a/b
			numturns++;
			ingametimer = 0;
			break;
		case choice_b:
			nokia_lcd_clear();
			nokia_lcd_write_string("choose jug to empty A/B ", 1);
			if (!showtimer) {
				nokia_lcd_write_string("    Timer = ",1);
				nokia_lcd_write_char((char)ingametimer/10 + 0x30,1);
				nokia_lcd_write_char((char)ingametimer%10 + 0x30,1);
			}
			else {
				nokia_lcd_write_string("              ",1);
			}
			nokia_lcd_write_string("                  ",1);
			nokia_lcd_write_string("X to go back",1);
			
			ingametimer++;
			break;
		case snes_x: // fill a/b
			numturns++;
			ingametimer = 0;
			break;
		case choice_x:
			nokia_lcd_clear();
			nokia_lcd_write_string("choose jug to fill A/B  ", 1);
			if (!showtimer) {
				nokia_lcd_write_string("    Timer = ",1);
				nokia_lcd_write_char((char)ingametimer/10 + 0x30,1);
				nokia_lcd_write_char((char)ingametimer%10 + 0x30,1);
			}
			else {
				nokia_lcd_write_string("              ",1);
			}
			nokia_lcd_write_string("                  ",1);
			nokia_lcd_write_string("X to go back",1);
			
			ingametimer++;
			break;
		case snes_y: // toggle timer display
			if (!showtimer)
				showtimer++;
			else
				showtimer--;
			break;
		case gameover:
			nokia_lcd_clear();
			if (!gamewin) 
				nokia_lcd_write_string("Gameover",2);
			else 
				nokia_lcd_write_string("Congrats",2);
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
			capA = 0;
			capB = 0;
			maxA = 0;
			maxB = 0;
			break;
	}
}

int main(void) {
	DDRA = 0xFE; PORTA = 0x01;
	DDRB = 0xFF; PORTB = 0x00;

	// unsigned char output = 0x00;

	// reference giuseppi for lcd code
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
