/*	Author: brandon babu
 *  Partner(s) Name: 
 *	Lab Section: 027
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    ADC_init();
    unsigned short s = ADC;
    unsigned char c, d;
    unsigned short max = 0x02FF;
    while (1) {
	    s = ADC;
	    if (s >= max/2) {
		    c = 0x01;
	    }
	    else {
		    c = 0x00;
	    }
	    //c = (char)s;
	    //d = (char) ((s >> 4) & 0x0F);
	    PORTB = c;
	    //PORTD = d;
    }
    return 1;
}
