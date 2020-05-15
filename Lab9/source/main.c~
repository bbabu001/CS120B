/*	Author: brandon babu
 *  Partner(s) Name: 
 *	Lab Section:027
 *	Assignment: Lab #9  Exercise #2
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

void set_PWM(double frequency) {
	static double current_frequency; // keeps track of currently set frequency
	// will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } // stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char a, i, check;
double note[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};


enum scale_states{Start, init, up, down} scale_state;

void Scale_Tick() {
	switch(scale_state) { //state transitions
		case Start:
			scale_state = init;
			break;
		case init:
			if (a == 0x02) {
				scale_state = up;
			}
			else if (a == 0x04) {
				scale_state = down;
			}
			else {
				scale_state = init;
			}
			break;
		case up:
			if (a == 0x02) {
				scale_state = up;
			}
			else {
				scale_state = init;
			}
			break;
		case down:
			if (a == 0x04) {
				scale_state = down;
			}
			else {
				scale_state = init;
			}
			break;
		default:
			scale_state = init;
			break;
	}
	switch (scale_state) { // state actions
		case Start:
			break;
		case init:
			check = 0;
			set_PWM(note[0]);
			break;
		case up:
			if (check == 0 && i < 7) {
				i++;
				check = 1;
			}
			set_PWM(note[i]);
			break;
		case down:
			if (check == 0 && i > 0) {
				i--;
				check = 1;
			}
			set_PWM(note[i]);
			break;
		default:
			break;
	}
}

enum sound_states{off, on, wait}sound_state;
void Sound_Tick() {
	switch(sound_state) { // state transitions
		case off:
			if (a == 0x01) {
				sound_state = on;
			}
			else if (a == 0x00) {
				sound_state = off;
			}
			break;
		case on:
			if (a == 0x00) {
				sound_state = wait;
			}
			else if (a == 0x01) {
				sound_state = on;
			}
			break;
		case wait:
			if (a == 0x00) {
				sound_state = wait;
			}
			else if (a == 0x01) {
				sound_state = off;
			}
			break;
		default:
			sound_state = off;
			break;
	}
	switch(sound_state) { // state transitions
		case off:
			PWM_off();
			break;
		case on:
			PWM_on();
			set_PWM(note[i]);
			break;
		case wait:
			PWM_on();
			set_PWM(note[i]);
			break;
		default:
			PWM_off();
			break;
	}
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;

    a = 0;
    i = 0;
    check = 0;
    scale_state = Start;
    sound_state = off;
    TimerSet(50);
    TimerOn();
    while (1) {
	    a = ~PINA & 0x07;
	    Scale_Tick();
	    Sound_Tick();

	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
