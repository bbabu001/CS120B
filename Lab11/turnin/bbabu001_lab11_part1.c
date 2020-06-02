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
//#include <queue.h>
//#include <scheduler.h>
//#include <seven_seg.h>
//#include <stack.h>
#include <timer.h>
#endif

//find GCD function
unsigned long int findGCD(unsigned long int a, unsigned long int b) {
	unsigned long int c;
	while(1) {
		c = a%b;
		if (c==0) {return b;}
		a = b;
		b = c;
	}
	return 0;
}

// task scheduler data structure
typedef struct task{
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;
// end task data structure

// shared variables
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;
unsigned char i;

unsigned char x = 0;
unsigned char keyout = 0;
// end shared variables

enum pasueButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release};
int pauseButtonSMTick(int state) {
	unsigned char press = ~PINA & 0x01;
	switch(state) {
		case pauseButton_wait:
			state = press == 0x01? pauseButton_press: pauseButton_wait; break;
		case pauseButton_press:
			state = pauseButton_release; break;
		case pauseButton_release:
			state = press == 0x00? pauseButton_wait: pauseButton_press; break;
		default: state = pauseButton_wait; break;
	}
	switch(state) {
		case pauseButton_wait: break;
		case pauseButton_press: 
			pause = (pause == 0) ? 1 : 0;
			break;
		case pauseButton_release: break;
	}
	return state;
}

enum toggleLED0_States {toggleLED0_wait, toggleLED0_blink};
int toggleLED0SMTick(int state) {
	switch(state) {
		case toggleLED0_wait: state = !pause? toggleLED0_blink: toggleLED0_wait; break;
		case toggleLED0_blink: state = pause? toggleLED0_wait: toggleLED0_blink; break;
		default: state = toggleLED0_wait; break;
	}
	switch(state) {
		case toggleLED0_wait: break;
		case toggleLED0_blink: 
			led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return state;
}

enum toggleLED1_States {toggleLED1_wait, toggleLED1_blink};
int toggleLED1SMTick(int state) {
	switch(state) {
		case toggleLED1_wait: state = !pause? toggleLED1_blink: toggleLED1_wait; break;
		case toggleLED1_blink: state = pause? toggleLED1_wait: toggleLED1_blink; break;
		default: state = toggleLED1_wait; break;
	}
	switch(state) {
		case toggleLED1_wait: break;
		case toggleLED1_blink: 
			led1_output = (led1_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return state;
}

enum display_States {display_display};
int displaySMTick(int state) {
	unsigned char output;
	switch(state) {
		case display_display: state = display_display; break;
		default: state = display_display; break;
	}
	switch(state) {
		case display_display: 
			output = led0_output | led1_output << 1;
			break;
	}
	PORTB = output;
	return state;
}

enum keypad_states {keypad};
int keypadSMTick(int state) {
        x = GetKeypadKey();
        switch(state) {
                case keypad:
                        state = keypad;
                        break;
                default:
                        state = keypad;
                        break;
        }
        switch(state) {
                case keypad:
                        switch(x) {
                                case '\0': keyout = '\0'; break;
                                case '1': keyout = '1'; break;
                                case '2': keyout = '2'; break;
                                case '3': keyout = '3'; break;
                                case '4': keyout = '4'; break;
                                case '5': keyout = '5'; break;
                                case '6': keyout = '6'; break;
                                case '7': keyout = '7'; break;
                                case '8': keyout = '8'; break;
                                case '9': keyout = '9'; break;
                                case 'A': keyout = 'A'; break;
                                case 'B': keyout = 'B'; break;
                                case 'C': keyout = 'C'; break;
                                case 'D': keyout = 'D'; break;
                                case '*': keyout = '*'; break;
                                case '0': keyout = '0'; break;
                                case '#': keyout = '#'; break;
                                default: keyout = 0x1B; break;
                        }
        }
        return state;
}

/*
enum keypad_states {keypad};
int keypadSMTick(int state) {
	x = GetKeypadKey();
	switch(state) {
		case keypad:
			state = keypad;
			break;
		default:
			state = keypad;
			break;
	}
	switch(state) {
		case keypad: 
			switch(x) {
	       			case '\0': keyout = 0x1F; break; 
      	      			case '1': keyout = 0x01; break; 
        		        case '2': keyout = 0x02; break;
                		case '3': keyout = 0x03; break;
	               		case '4': keyout = 0x04; break;
        	      		case '5': keyout = 0x05; break;
                		case '6': keyout = 0x06; break;
	                	case '7': keyout = 0x07; break;
        	        	case '8': keyout = 0x08; break;
                		case '9': keyout = 0x09; break;
	                	case 'A': keyout = 0x0A; break;
        	        	case 'B': keyout = 0x0B; break;
                		case 'C': keyout = 0x0C; break;
	                	case 'D': keyout = 0x0D; break;
        	        	case '*': keyout = 0x0E; break;
	                	case '0': keyout = 0x00; break;
        	        	case '#': keyout = 0x0F; break;
                		default: keyout = 0x1B; break; 
			}
	}
	return state;
}
*/

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00; // PORTB set to output
    DDRC = 0xF0; PORTC = 0x0F; // PC7-4 set to output, PC3-0 set to input
    DDRD = 0xFF; PORTD = 0x00; // PORTD LCD output

    LCD_init();
    
    static task task1, task2, task3, task4, task5;
    task *tasks[] = { &task1, &task2, &task3, &task4, &task5 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 (pauseButtonToggleSM)
    task1.state = pauseButton_wait; //initial state
    task1.period = 50; //period
    task1.elapsedTime = task1.period; //current elapsed time
    task1.TickFct = &pauseButtonSMTick; //function pointer for tick
    //Task2 (toggleLED0SM)
    task2.state = toggleLED0_wait;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &toggleLED0SMTick;
    //Task3 (toggleLED1SM)
    task3.state = toggleLED1_wait;
    task3.period = 1000;
    task3.elapsedTime = task3.period;
    task3.TickFct = &toggleLED1SMTick;
    //Task4 (displaySM)
    task4.state = display_display;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &displaySMTick;
    //Task5 (keypadSM)
    task5.state = keypad;
    task5.period = 50;
    task5.elapsedTime = task5.period;
    task5.TickFct = &keypadSMTick;

    unsigned long GCD = tasks[0]->period;
    for(i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD,tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();

    while (1) {
	    for (i = 0; i < numTasks; i++) { //scheduler code
		    if (tasks[i]->elapsedTime == tasks[i]->period) { // task is ready to tick
			    tasks[i]->state = tasks[i]->TickFct(tasks[i]->state); // set next state
			    tasks[i]->elapsedTime = 0; // reset elapsed time for next tick
		    }
		    tasks[i]->elapsedTime += GCD;
	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
