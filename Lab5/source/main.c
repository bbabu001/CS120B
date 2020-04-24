/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, init, s1, s2, s3, s4} state;
unsigned char tmpA;
unsigned char tmpB;
unsigned char press;

void Tick() {
	switch(state) {// Transitions
		case Start:
			state = init;
			break;
		case init:
			if (tmpA == 0x01 && press == 1) {
				state = s1;
				press = 0;
				tmpB = 0x21;
			}
			else {
				state = init;
				tmpB = 0x00;
			}
			if (tmpA == 0x00) {
				press = 1;
			}
			break;
		case s1:
			if (tmpA == 0x01 && press == 1) {
				state = s2;
				press = 0;
				tmpB = 0x12;
			}
			else {
				state = s1;
				tmpB = 0x21;
			}
			if (tmpA == 0x00) {
				press = 1;
			}
			break;
		case s2:
			if (tmpA == 0x01 && press == 1) {
                                state = s3;
                                press = 0;
				tmpB = 0x0C;
                        }
                        else {
                                state = s2;
				tmpB = 0x12;
                        }
                        if (tmpA == 0x00) {
                                press = 1;
                        }
                        break;
		case s3:
			if (tmpA == 0x01 && press == 1) {
                                state = s4;
                                press = 0;
				tmpB = 0x3F;
                        }
                        else {
                                state = s3;
				tmpB = 0x0C;
                        }
                        if (tmpA == 0x00) {
                                press = 1;
                        }
                        break;
		case s4:
			if (tmpA == 0x01 && press == 1) {
                                state = init;
                                press = 0;
				tmpB = 0x00;
                        }
                        else {
                                state = s4;
				tmpB = 0x3F;
                        }
                        if (tmpA == 0x00) {
                                press = 1;
                        }
                        break;
		default:
			state = init;
			press = 0;
			tmpB = 0x00;
			break;

	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	tmpA = 0x00;
	tmpB = 0x00;
	press = 0;
	state = init;

    while (1) {
	tmpA = (~PINA & 0x01);
	Tick();
	PORTB = tmpB;
    }
    return 0;
}
