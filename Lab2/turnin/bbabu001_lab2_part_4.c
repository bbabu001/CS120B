/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #2  Exercise #3
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;

	unsigned char total = 0x00;
	unsigned char total2 = 0x00;
    	unsigned char diff = 0x00;
	unsigned char tmpD = 0x00;

    while (1) {
	total = 0x00;
	total2 = 0x00;
	diff = 0x00;
	tmpD = 0x00;
	total = PINA + PINB + PINC;
	total2 = total;

	if (PINA > PINC) {
		diff = PINA - PINC;
	}
	else {
		diff = PINC - PINA;
	}

	if (diff > 80) {
		tmpD = (tmpD & 0xFD) | 0x02;
	}
	else {
		tmpD = (tmpD & 0xFD) | 0x00;
	}

	if (total <= 127 && total > 63) {
		tmpD = (tmpD & 0x03) | (total << 1);
		tmpD = (tmpD & 0xFE) | 0x00;
	}
	else if (total <= 63) {
		tmpD = (tmpD & 0x03) | (total << 2);
		tmpD = (tmpD & 0xFE) | 0x00;
	}
	else if (total <= 140 && total > 127) {
		tmpD = ((tmpD & 0x03) | (total2 & 0xFC));
		tmpD = (tmpD & 0xFE) | 0x00;
	}
	else {
		tmpD = ((tmpD & 0x03) | (total2 & 0xFC));
		tmpD = (tmpD & 0xFE) | 0x01;
	}
	PORTD = tmpD;
    }
    return 0;
}
