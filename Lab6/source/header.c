#include <avr/io.h>
#include <avr/interrupt.h>

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

void main() {

	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	unsigned char tempB = 0x00;
	while(1){
		//User code (i.e synchSM calls)
		tempB = ~tempB; //Toggle PORTB; Temporary, bad programmng style
		PORTB = tempB;
		while(!TimerFlag); //Wait 1 sec
		TimerFlag = 0;
		//Note: For the above a better style would use a synchSM with TickSM()
		//This example just illustrates the use of the ISR and flag

	}
}