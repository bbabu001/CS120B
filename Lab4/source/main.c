/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section: 027
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, OffRelease, OffPress, OnRelease, OnPress} state;
unsigned char a;
unsigned char b;

void Tick() {

	switch(state) { // Transitions
		case Start:
			state = OffRelease;
			break;
		case OffRelease:
			state = a ? OffPress : OffRelease;
			break;
		case OffPress:
			state = a ? OffPress : OnRelease;
			break;
		case OnRelease:
			state = a ? OnPress : OnRelease;
			break;
		case OnPress:
			state = a ? OnPress : OffRelease;
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {	// State Actions
		case Start:
			break;
		case OffRelease:
			b = 0x01;
			break;
		case OffPress:
			b = 0x02;
			break;
		case OnRelease:
			break;
		case OnPress:
			b = 0x01;
			break;		
		default:
			break;	
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
    while (1) {
	b = 0x00;
	state = Start;
	a = PINA;
	Tick();
	PORTB = b;
    }
    return 0;
}
