/*	Author: brandon babu
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency; // keeps track of currently set frequency
	// will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } // stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // updates the current frequency
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

enum States{Start, p1, p2, off, on, up, down} state;
unsigned char a, i, check;
double note[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
void Tick() {
	switch(state) { //state transitions
		case Start:
			check = 0;
			state = off;
			break;
		case off:
			if (a == 0x01) {
				state = p1;
			}
			else {
				state = off;
			}
			break;
		case p1:
			if (a == 0x01) {
				state = p1;
			}
			else {
				state = on;
			}
			break;
		case on:
			if (a == 0x01) {
				state = p2;
			}
			else if (a == 0x02) {
				state = up;
			}
			else if (a == 0x04) {
				state = down;
			}
			else {
				state = on;
			}
			break;
		case p2:
			if (a == 0x01) {
				state = p2;
			}
			else {
				state = off;
			}
			break;
		case up:
			if (a == 0x02) {
				state = up;
			}
			else {
				state = on;
			}
			break;
		case down:
			if (a == 0x04) {
				state = down;
			}
			else {
				state = on;
			}
			break;
		default:
			check = 0;
			state = off;
			break;
	}
	switch (state) { // state actions
		case Start:
			check = 0;
			PWM_on();
			set_PWM(note[0]);
			break;
		case off:
			PWM_off();
			check = 1;
			break;
		case on:
			PWM_on();
			set_PWM(note[i]);
			check = 0;
			break;
		case p1:
			break;
		case p2:
			break;
		case up:
			if (check ==  0 && i < 7) {
                                i++;
                                check = 1;
                        }
			set_PWM(note[i]);
			break;
		case down:
			if (check == 0 && i > 0) {
                                i--;
                                check = 1;
                        }
			set_PWM(note[i]);
			break;
		default:
			PWM_on();
			break;
	}
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;

    a = 0;
    i = 0x00;
    check = 0;
    state = Start;
    PWM_on();
    while (1) {
	    a = ~PINA & 0x07;
	    Tick();
    }
    return 1;
}
