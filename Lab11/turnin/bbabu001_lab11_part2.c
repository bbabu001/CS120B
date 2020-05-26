/*	Author: brandon
 *  Partner(s) Name: 
 *	Lab Section: 027
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <io.h>
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
unsigned char i;
unsigned char cnt = 0;
unsigned char pos = 0;
unsigned char x = 0;
unsigned char keyout = 0;
// end shared variables

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

enum lcd_states {display};
int lcdSMTick(int state) {
	unsigned char msg[40] = {' ', ' ', ' ', 'C', 'S', '1', '2', '0', 'B', ' ', 'i', 's', ' ', 'L', 'e', 'g', 'e', 'n', 'd', '.', '.', '.', ' ', 'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', ' '};
	switch(state) {
		case display:
			state = display;
			break;
		default: 
			state = display;
			break;
	}
	switch(state) {
		case display:
			if (pos < 24) {
				for(cnt = 1; cnt <= 16; cnt++) {
					LCD_Cursor(cnt);
					LCD_WriteData(msg[pos+cnt]);
				}
				pos++;
			}
			else {
				pos = 0;
			}
			break;
	}
	return state;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00; // PORTB set to output
    DDRC = 0xF0; PORTC = 0x0F; // PC7-4 set to output, PC3-0 set to input
    DDRD = 0xFF; PORTD = 0x00; // PORTD LCD output

    LCD_init();
    
    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 (lcdSM)
    task1.state = display; //initial state
    task1.period = 300; //period
    task1.elapsedTime = task1.period; //current elapsed time
    task1.TickFct = &lcdSMTick; //function pointer for tick

    unsigned long GCD = tasks[0]->period;
    for(i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD,tasks[i]->period);
    }

    TimerSet(300);
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
    return 0;
}
