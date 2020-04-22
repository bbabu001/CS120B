/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #5  Exercise #1
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

    while (1) {
	tmpA = 0x00;
	tmpC = 0x00;
	
	tmpA = PINA & 0x0F;
	switch (tmpA) {
		case 0:
			tmpC = 0x40;
			break;
		case 1:
			tmpC = 0x60;
		case 2:
			tmpC = 0x60;
			break;
		case 3:
			tmpC = 0x70;
		case 4:
			tmpC = 0x70;
			break;
		case 5:
			tmpC = 0x38;
		case 6:
			tmpC = 0x38;
			break;
		case 7:
			tmpC = 0x3C;
		case 8:
			tmpC = 0x3C;
		case 9:
			tmpC = 0x3C;
			break;
		case 10:
			tmpC = 0x3E;
		case 11:
			tmpC = 0x3E;
		case 12:
			tmpC = 0x3E;
			break;
		case 13:
			tmpC = 0x3F;
		case 14:
			tmpC = 0x3F;
		case 15:
			tmpC = 0x3F;
			break;
	}
	
	PORTC = tmpC;
    }
    return 0;
}
