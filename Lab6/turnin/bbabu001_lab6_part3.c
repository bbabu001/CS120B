/*	Author: bbabu001
 *  Partner(s) Name: 
 *	Lab Section: 027
 *	Assignment: Lab #6  Exercise #3
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

enum States {Start, init, pressA0, pressA1, wait, bothPress} state;
unsigned char a;
unsigned char b;
unsigned char t;

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

void Tick() {

	switch(state) { // Transitions
		case Start:
			state = init;
		case init:
			if (a == 0x01) {
				state = pressA0;
				if (b < 9) { b++; }
			}
			else if (a == 0x02) {
				state = pressA1;
				if (b > 0) { b--; }
			}
			else if (a == 0x03) {state = bothPress; }
			else { state = init; }
			break;
		case pressA0:
			if (a == 0x00) { state = wait; }
			else if (a == 0x03) { state = bothPress; }
			else { state = pressA0; }
			break;
		case pressA1:
			if (a == 0x00) { state = wait; }
			else if (a == 0x03) { state = bothPress; }
			else { state = pressA1; }
			break;
		case wait:
			if (a == 0x01) {
                                state = pressA0;
				if (b < 9) { b++; }
                        }
			else if (a == 0x02) {
                                state = pressA1;
				if (b > 0) { b--; }
			}
			else if (a == 0x03) {state = bothPress; }
                        else { state = wait; }
			break;
		case bothPress:
			if (a == 0x03) { state = bothPress; }
			else { state = wait; }
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {	// State Actions
		case Start:
			b = 0x07;
			break;
		case init:
			b = 0x07;
			t = 0;
			break;
		case pressA0:
			if (b < 9 && t == 10) {
				b++;
				t = 0;
			}
			t++;
			break;
		case pressA1:
			if (b >= 1 && t == 10) {
				b--;
				t = 0;
			}
			t++;
			break;
		case wait:
			t = 0;
			break;
		case bothPress:
			b = 0x00;
			t = 0;
			break;		
		default:
			b = 0x07;
			t = 0;
			break;	
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	b = 0x07;
	state = init;
    while (1) {
	a = ~PINA & 0x03;
	Tick();
	while(!TimerFlag); //Wait 1 sec
	TimerFlag = 0;
	PORTB = b;
    }
    return 0;
}
