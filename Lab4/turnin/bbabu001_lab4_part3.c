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

enum States {Start, init, wait, hPress, open} state;
unsigned char a;
unsigned char b;
unsigned char check;

void Tick() {

	switch(state) { // Transitions
		case Start:
			state = init;
		case init:
			if (a == 0x04) {
				state = hPress;
				check = 1;
			}
			else {
				state = init;
			}
			break;
		case wait:
			if (a == 0x02 && check == 1) {
				state = open;
			}
			else if (a == 0x04 && check == 0) {
				state = hPress;
			}
			else {
				state = wait;
			}
			break;
		case hPress:
			if (a == 0x00) {
				state = wait;
			}
			else { 
				state = hPress;
			}
			break;
		case open:
			if (a == 0x80) {
				state = init;
			}
			else {
				state = open;
			}
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {	// State Actions
		case Start:
			b = 0x00;
			check = 0;
			break;
		case init:
			b = 0x00;
			check = 0;
			break;
		case wait:
			if (check == 1 && (a == 0x01 || 0x04)) {
				check = 0;
			}
			else {
				check = 1;
			}
			break;
		case hPress:
			break;
		case open:
			b = 0x01;
			break;
		default:
			b = 0x00;
			check = 0;
			break;	
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	b = 0x00;
	state = init;
	check = 0;
    while (1) {
	a = PINA;
	Tick();
	PORTB = b;
    }
    return 0;
}
