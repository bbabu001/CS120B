/*	Author: brandon babu
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #10  Exercise #1
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
        //      TimerISR will be called every _avr_timer_cntcurr milliseconds

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

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char s, a, t;

enum ThreeStates{Start, zero, one, two}ThreeState;
void ThreeLEDTick() {
	switch(ThreeState) { // state transitions
		case Start:
			ThreeState = zero;
			break;
		case zero:
			ThreeState = one;
			break;
		case one:
			ThreeState = two;
			break;
		case two:
			ThreeState = zero;
			break;
		default:
			ThreeState = zero;
			break;
	}
	switch(ThreeState) { // state actions
		case Start:
			break;
		case zero:
			threeLEDs = 0x01;
			break;
		case one:
			threeLEDs = 0x02;
			break;
		case two:
			threeLEDs = 0x04;
			break;
		default:
			break;
	}
}

enum BlinkStates{on, off}BlinkState;
void BlinkLEDTick() {
	switch(BlinkState) {
		case on:
			BlinkState = off;
			break;
		case off:
			BlinkState = on;
			break;
		default:
			BlinkState = on;
			break;
	}
	switch(BlinkState) {
		case on:
			blinkingLED = 0x08;
			break;
		case off:
			blinkingLED = 0x00;
			break;
		default:
			break;
	}
}

enum CombineStates{out}CombineState;
void CombineLEDTick() {
	switch(CombineState) {
		case out:
			CombineState = out;
			break;
		default:
			CombineState = out;
			break;
	}
	switch(CombineState) {
		case out:
			PORTB = threeLEDs | blinkingLED;
			break;
		default:
			break;
	}
}

enum SpeakerStates{init, up, down}SpeakerState;
void SpeakerTick() {
	switch(SpeakerState) {
		case init:
			if (a == 0x04) {
				SpeakerState = up;
				t = 0;
			}
			else {
				SpeakerState = init;
			}
			break;
		case up:
			if (a == 0x04 && t < 2) {
				SpeakerState = up;
				t++;
			}
			else if (a == 0x04 && t >= 2) {
				SpeakerState = down;
				t = 0;
			}
			else {
				SpeakerState = init;
			}
			break;
		case down:
			if (a == 0x04 && t< 2) {
				SpeakerState = down;
				t++;
			}
			else if (a == 0x04 && t >= 2) {
				SpeakerState = up;
				t = 0;
			}
			else {
				SpeakerState = init;
			}
			break;
		default:
			SpeakerState = init;
			break;
	}
	switch(SpeakerState) {
		case init:
			s = 0x00;
			break;
		case up:
			s = 0x10;
			break;
		case down:
			s = 0x00;
			break;
		default:
			break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;

	unsigned long Th_elapsedTime = 0;
	unsigned long Bl_elapsedTime = 0;
	const unsigned long timerPeriod = 1;

	TimerSet(1);
	TimerOn();

	threeLEDs = 0x00;
	blinkingLED = 0x00;
	s = 0;
	a = 0;
	t = 0;

	ThreeState = zero;
	BlinkState = on;
	CombineState = out;
	SpeakerState = init;

    while (1) {
	    a = ~PINA & 0x04;

	    if (Th_elapsedTime >= 300) {
		    ThreeLEDTick();
		    Th_elapsedTime = 0;
	    }
	    if (Bl_elapsedTime >= 1000) {
		    BlinkLEDTick();
		    Bl_elapsedTime = 0;
	    }
	    CombineLEDTick();
	    SpeakerTick();

	    while (!TimerFlag);
	    TimerFlag = 0;

	    Th_elapsedTime += timerPeriod;
	    Bl_elapsedTime += timerPeriod;
    }
    return 1;
}
