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
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
    	unsigned char tmpC = 0x00;
	unsigned char cntavail = 0x00;
	unsigned char i;

    while (1) {
	cntavail = 0x00;
	for (i = 0; i < 4; i++) {
		tmpA = PINA >> i;
		if ((tmpA & 0x01) == 0x00){ 
			cntavail++;
		}
	}
	tmpC = cntavail;
	if (cntavail == 0x00) {
		tmpC = (tmpC & 0x7F) | 0x80;
	}
	else {
		tmpC = (tmpC & 0x7F) | 0x00;
	}
	PORTC = tmpC;
    }
    return 0;
}
