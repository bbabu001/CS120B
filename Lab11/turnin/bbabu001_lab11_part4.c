/*	Author: brandon
 *  Partner(s) Name: 
 *	Lab Section: 027
 *	Assignment: Lab #11  Exercise #3
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
unsigned char x = 0;
unsigned char keyout = 0;
unsigned char cnt = 0;
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

enum lcd_states {display, press};
int lcdSMTick(int state) {
	static char temp = 0;
	static char msg[16] = {'C', 'o', 'n', 'g', 'r', 'a', 't', 'u', 'l', 'a', 't', 'i', 'o', 'n', 's', '!'};
	switch(state) {
		case display:
			if (keyout == '\0') {
				state = display;
			}
			else {
				state = press;
			}
			break;
		case press:
			if (keyout == '\0') {
				state = display;
				temp++;
				//if (temp < 16) {
                                //	temp++;
                        	//}
                        	//else {
                                //	temp = 0;;
                        	//}
			}
			else {
				state = press;
			}
			break;
		default: 
			state = display;
			break;
	}
	switch(state) {
		case display:
			for(cnt = 1; cnt <= 16; cnt++) {
				LCD_Cursor(cnt);
				LCD_WriteData(msg[cnt-1]);
			}
			
			break;
		case press:
			msg[temp%16] = keyout;
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
    
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 (lcdSM)
    task1.state = display; //initial state
    task1.period = 250; //period
    task1.elapsedTime = task1.period; //current elapsed time
    task1.TickFct = &lcdSMTick; //function pointer for tick

    //Task 2 (keypadSM)
    task2.state = keypad;
    task2.period = 10;
    task2.elapsedTime = task2.period;
    task2.TickFct = &keypadSMTick;

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
    return 0;
}
