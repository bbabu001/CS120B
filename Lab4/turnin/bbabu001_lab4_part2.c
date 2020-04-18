/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section: 027
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, init, pressA0, pressA1, wait, bothPress} state;
unsigned char a;
unsigned char c;
unsigned char check;

void Tick() {

	switch(state) { // Transitions
		case Start:
			state = init;
		case init:
			if (a == 0x01) {
				state = pressA0;
			}
			else if (a == 0x02) {
				state = pressA1;
			}
			else if (a == 0x03) {state = bothPress; }
			else { state = init; }
			break;
		case pressA0:
			if (a == 0x00) { state = wait; }
			else if (a == 0x03) { state = bothPress; }
			else { state = pressA0; }
			break;
		case pressA1:
			if (a == 0x00) { state = wait; }
			else if (a == 0x03) { state = bothPress; }
			else { state = pressA1; }
			break;
		case wait:
			if (a == 0x01) {
                                state = pressA0;
                        }
			else if (a == 0x02) {
                                state = pressA1;
			}
			else if (a == 0x03) {state = bothPress; }
                        else { state = wait; }
			break;
		case bothPress:
			if (a == 0x03) { state = bothPress; }
			else { state = wait; }
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {	// State Actions
		case Start:
			c = 0x07;
			check = 0;
			break;
		case init:
			c = 0x07;
			check = 0;
			break;
		case pressA0:
			if (c < 9 && check == 0) {
				c++;
				check = 1;
			}
			break;
		case pressA1:
			if (c >= 1 && check == 0) {
				c--;
				check = 1;
			}
			break;
		case wait:
			check = 0;
			break;
		case bothPress:
			c = 0x00;
			check = 0;
			break;		
		default:
			c = 0x07;
			check = 0;
			break;	
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	c = 0x07;
	state = init;
	check = 0;
    while (1) {
	a = PINA;
	Tick();
	PORTC = c;
    }
    return 0;
}
