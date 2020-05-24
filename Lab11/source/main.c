/*	Author: brandon
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <io.c>
#include <keypad.h>
//#include <lcd_8bit_task.h>
#include <queue.h>
#include <scheduler.h>
#include <seven_seg.h>
#include <stack.h>
#include <timer.h>
#endif


int main(void) {
    unsigned char x;
    DDRB = 0xFF; PORTB = 0x00; // PORTB set to output
    DDRC = 0xF0; PORTC = 0x0F; // PC7-4 set to output, PC3-0 set to input
    
    while (1) {
	    x = GetKeypadKey();
	    switch(x) {
		case '\0': PORTB = 0x1F; break; // all 5 LEDS on
		case '1': PORTB = 0x01; break; // hex eqivalent
		case '2': PORTB = 0x02; break;
		case '3': PORTB = 0x03; break;
		case '4': PORTB = 0x04; break;
		case '5': PORTB = 0x05; break;
		case '6': PORTB = 0x06; break;
		case '7': PORTB = 0x07; break;
		case '8': PORTB = 0x08; break;
		case '9': PORTB = 0x09; break;
		case 'A': PORTB = 0x0A; break;
		case 'B': PORTB = 0x0B; break;
		case 'C': PORTB = 0x0C; break;
		case 'D': PORTB = 0x0D; break;
		case '*': PORTB = 0x0E; break;
		case '0': PORTB = 0x00; break;
		case '#': PORTB = 0x0F; break;
		default: PORTB = 0x1B; break; // should never occur. Middle LED off.
	    }
    }
    return 1;
}
