/*	Author: brandon babu
 *  Partner(s) Name: 
 *	Lab Section: 27
 *	Assignment: Lab #6  Exercise #2
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
enum States {Start, s1, s2, s3, hold1, hold2} state;
unsigned char b;
unsigned char a;
unsigned char dir;
unsigned char press;

volatile unsigned char TimerFlag = 0; //TimerISR() sets this to 1. C programmer should clear to 0

//internal variables for mapping AVR's ISR to our cleaner TimerISR() model.
unsigned long _avr_timer_M = 1; //start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; //Current internal count of 1ms ticks

void TimerOn(){
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; 

	//AVR output compare register OCR1A
	OCR1A = 125; 

	//AVR timer interrupt mask register
	TIMSK1 = 0x02; //bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	//	TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; //0x80: 1000000

}

void TimerOff(){
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){

	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}

}

//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){

	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;

}

void TickSM() {
	switch(state) {
		case Start:
			state = s1;
			break;
		case s1:
			if (a == 0x01) {
				state = hold1;
				press = 1;
			}
			else {
				state = s2;
			}
			break;
		case s2:
			if (a == 0x01) {
				state = hold1;
				press = 1;
			}
			else {
				if (dir == 1) {
					state = s3;
				}
				else {
					state = s1;
				}
			}
			break;
		case s3:
			if (a == 0x01) {
				state = hold1;
				press = 1;
			}
			else {
				state = s2;
			}
			break;
		case hold1:
			if (a == 0x00 && press == 1) {
				state = hold2;
			}
			else if (a == 0x00 && press == 0) {
				if (b == 0x01) {
					state = s2;
					dir = 1;
				}
				else if (b == 0x02) {
					state = s2;
					//if (dir == 1) {
					//	state = s3;
					//}
					//else {
					//	state = s1;
					//}
				}
				else if (b == 0x04) {
					state = s2;
					dir = 1;
				}
			}
			break;
		case hold2:
			if (a == 0x01) {
				state = hold1;
				press = 0;
			}
			break;
		default:
			state = s1;
			break;
	}
	switch(state) {
		case Start:
			b = 0x01;
			dir = 0;
			break;
		case s1:
			b = 0x01;
			dir = 1;
			break;
		case s2:
			b = 0x02;
			break;
		case s3:
			b = 0x04;
			dir = 0;
			break;
		case hold1:
			break;
		case hold2:
			break;
		default:
			b = 0x01;
			break;
	}
}

int main() {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	b = 0x00;
	a = 0x00;
	state = Start;

	while(1){
		a = ~PINA & 0x01;
		TickSM();
		while(!TimerFlag); //Wait 1 sec
		TimerFlag = 0;
		PORTB = b;
	}
	return 0;
}
