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
unsigned char i = 0;
unsigned char x = 0;
unsigned char keyout = 0;
unsigned char a = 0;
unsigned char cnt = 0;
unsigned char pos1 = 0;
unsigned char pos2 = 0;
unsigned char updown = 0;
unsigned char pst = 0;
unsigned char lose = 0;
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

enum start_states {init, start, pause};
int startSMTick(int state) {
	switch(state) {
		case init:
			if (a == 0x01) {
				state = start;
			}
			else {
				state = init;
			}
			break;
		case start:
			if (a == 0x01 || lose == 1) {
				state = pause;
			}
			else {
				state = start;
			}
			break;
		case pause:
			if (a == 0x01) {
				state = start;
			}
			else {
				state = pause;
			}
			break;
		default:
			state = init;
			break;
	}
	switch(state) {
		case init:
			pst = 0;
			break;
		case start:
			pst = 1;
			break;
		case pause:
			pst = 0;
			break;
	}
	return state;
}

enum move_states {up, down};
int moveSMTick(int state) {
	switch(state) {
		case up:
			if (a == 0x04) {
				state = down;
			}
			else {
				state = up;
			}
			break;
		case down:
			if (a == 0x02) {
				state = up;
			}
			else {
				state = down;
			}
			break;
		default:
			state = up;
			break;
	}
	switch(state) {
		case up:
			updown = 1;
			break;
		case down:
			updown = 2;
			break;
	}
}

enum game_states {stop, go, p, lost};
int gameSMTick(int state) {
	static char obs[16] = {' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	static char obs2[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' '};
	static char dead[8] = {'Y', 'O', 'U', ' ', 'L', 'O', 'S', 'E'};
	switch(state) {
		case stop:
			if (pst == 1) {
				state = go;
			}
			else {
				state = stop;
			}
			break;
		case go:
			if (pst == 0) {
				state = p;
			}
			else if ((pos1 == 6 && updown == 1) || (pos2 == 10 && updown == 2)) {
				state = lost;
				LCD_ClearScreen();
				lose = 1;
			}
			else {
				state = go;
			}
			break;
		case p:
			if (pst == 1) {
				state = go;
			}
			else {
				state = p;
			}
			break;
		case lost:
			if (pst == 1) {
				state = go;
				LCD_ClearScreen();
				lose = 0;
			}
			else {
				lose = 1;
				state = lost;
			}
			break;
		default:
			state = stop;
			break;
	}
	switch(state) {
		case stop:
			for(cnt = 0; cnt < 16; cnt++) {
				LCD_Cursor(cnt+1);
				LCD_WriteData(obs[cnt]);
				LCD_Cursor(cnt+17);
				LCD_WriteData(obs2[cnt]);
			}
			LCD_Cursor(1);
			break;
		case go:
			if (pos1 < 16) {
				for(cnt = 1; cnt <= 16; cnt++) {
					LCD_Cursor(cnt);
					LCD_WriteData(obs[(pos1+cnt)%16]);
					LCD_Cursor(cnt+16);
					LCD_WriteData(obs2[(pos2+cnt)%16]);
				}
				if (updown == 1) {
					LCD_Cursor(1);
				}
				else if (updown == 2) {
					LCD_Cursor(17);
				}
				pos1++;
				pos2++;
			}
			else {
				pos1 = 0;
				pos2 = 0;
			}
			
			break;
		case p:
			for (cnt = 1; cnt <= 16; cnt++) {
				LCD_Cursor(cnt);
				LCD_WriteData(obs[(pos1+cnt)%16]);
				LCD_Cursor(cnt+16);
				LCD_WriteData(obs2[(pos2+cnt)%16]);
			}
			break;
		case lost:
			for (cnt = 1; cnt <= 8; cnt++) {
				LCD_Cursor(cnt);
				LCD_WriteData(dead[cnt-1]);
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
    
    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2, &task3 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 (startSM)
    task1.state = 0; //initial state
    task1.period = 10; //period
    task1.elapsedTime = task1.period; //current elapsed time
    task1.TickFct = &startSMTick; //function pointer for tick

    //Task 2 (moveSM)
    task2.state = 0;
    task2.period = 10;
    task2.elapsedTime = task2.period;
    task2.TickFct = &moveSMTick;

    //Task 3 (gameSM)
    task3.state = 0;
    task3.period = 200;
    task3.elapsedTime = task3.period;
    task3.TickFct = &gameSMTick;

    unsigned long GCD = tasks[0]->period;
    for(i = 1; i < numTasks; i++) {
        GCD = findGCD(GCD,tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();

    while (1) {
	    a = ~PINA & 0x07;
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
