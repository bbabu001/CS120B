/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #3  Exercise #1
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
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char i;
    while (1) {
	tmpA = 0x00;
	tmpB = 0x00;
	tmpC = 0x00;
	
	for (i = 0; i < 8; i++) {
		tmpA = PINA >> i;
		tmpB = PINB >> i;
		if ((tmpA & 0x01) == 0x01) {
			tmpC++;
		}
		if ((tmpB & 0x01) == 0x01) {
			tmpC++;
		}
	}
	PORTC = tmpC;
    }
    return 0;
}
