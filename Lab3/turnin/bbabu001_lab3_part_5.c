/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #3  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x01;

	unsigned char tmpB = 0x00;
	unsigned short tmpD = 0;
	unsigned short weight = 0;
    while (1) {
	tmpB = 0x00;
	tmpD = 0;
	weight = 0;
	tmpB = PINB & 0x01;
	tmpD = PIND;
	weight = (tmpD << 1) | tmpB;
	if (weight >= 70) {
		tmpB = (tmpB & 0xF9) | 0x02;
	}
	else if (weight > 5 && weight < 70) {
		tmpB = (tmpB & 0xf9) | 0x04;
	}
	else {
		tmpB = (tmpB & 0xF9) | 0x00;
	}
	PORTB = tmpB & 0xFE;
    }
    return 0;
}
