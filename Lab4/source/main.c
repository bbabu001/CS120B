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
			//state = a ? OffPress : OffRelease;
			if (a == 0x01) { state = OffPress; }
			else { state = OffRelease; }
			break;
		case OffPress:
			//state = a ? OffPress : OnRelease;
			if (a == 0x01) { state = OffPress; }
			else { state = OnRelease; }
			break;
		case OnRelease:
			//state = a ? OnPress : OnRelease;
			if (a == 0x01) { state = OnPress; }
			else { state = OnRelease; }
			break;
		case OnPress:
			//state = a ? OnPress : OffRelease;
			if (a == 0x01) { state = OnPress; }
			else { state = OffRelease; }
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {	// State Actions
		case Start:
			b = 0x00;
			break;
		case OffRelease:
			b = 0x01;
			break;
		case OffPress:
			b = 0x02;
			break;
		case OnRelease:
			b = 0x02;
			break;
		case OnPress:
			b = 0x01;
			break;		
		default:
			b = 0x01;
			break;	
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	b = 0x00;
	state = Start;

    while (1) {
	a = PINA;
	Tick();
	PORTB = b;
    }
    return 0;
}
