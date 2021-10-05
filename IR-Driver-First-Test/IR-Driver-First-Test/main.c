/*
 * IR-Driver-First-Test.c
 *
 * Created: 03/10/2021 15:20:02
 * Author : mehme
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

int counter = 0;
// check every 1ms for value
//write a state machine
//change state if we are at IDLE state and input port is 0
//Read values every corresponding ms
//after some time change state to IDLE again
// hold time in counter value
//use shift register to store values
//after 8 bits, if that register is different than 0xFF and corresponding value,
// increment or decrement .
//show output at

// 9+4.5 ms Start State
//27ms Address State (does not atter ??) 
//27ms Command State
typedef enum{
	IDLE = 0,
	START = 1,
	ADDRESS = 2,
	COMMAND = 3,
	END = 4
}STATE;
typedef struct STATE_INFO {
	STATE currentState;
}STATE_INFO;
STATE_INFO State;

uint8_t my_register = 0;
int Total_time = 0;
int bit_counter = 0;
int counter_time = 0;
int main(void)
{
	TCCR0A = (1<<WGM01);
	OCR0A = 2;
	TCCR0B = (1<<CS01); // clk/8 prescalar. to count 1us each clock
	TIMSK0 = (1<<OCIE0A);
	
	DDRD = (1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5) | (1<<PORTD4) ; // For Red LED's, mode indication
	DDRB = (1<<PORTB5)|(0<<PORTB3); // Green LED for ON/OFF indication , PORTB3 as input
	PORTD &= 0b00001111;
	PORTB = 0b11011111; //PORT B3 initially high
	State.currentState = IDLE; // Start at IDLE state initially
	
    while (1) 
    {
		if(my_register == 0x18){
			PORTD &= 0b00001111; // close all led's first
			PORTD |= (1<<PORTD7);// open first led only
		}
		if(my_register == 0x4A){
			PORTD &= 0b00001111; // close all led's first
			PORTD |= (1<<PORTD6);// open first led only
		}
		if(my_register == 0x38){
			PORTD &= 0b00001111;
			PORTB ^= (1<<PORTB5);
		}
		
    }
}

ISR(TIMER0_COMPA_vect){
	
	
	switch(State.currentState){
		case(IDLE):
			if(!(PINB3 & 0x02)){ // get the value at input PORTB3 and check if it is 0
				State.currentState = START;
				
			}
			break;
		case(START):
			PORTB |= (1<<PORTB5);
			Total_time++;
			if(Total_time==13500){
				State.currentState = ADDRESS;
				}//13.5 ms 
			break;
		case(ADDRESS):
			Total_time++;
			if(Total_time == 40500){
				State.currentState = COMMAND;
			}
			break;
		case(COMMAND):
			Total_time++;
			counter_time++;
			if(bit_counter == 8){
				State.currentState = END;
			}
			if(counter_time==1200 && !(PINB3 & 0x02)){ // if it is 0 -> prev is logical 0 
				counter_time = counter_time - 1125; // reset counter timer for next bit. If it is logical 0 then previous was 1.125 ms long
				my_register = my_register<<1; // 0 added automatically
				
				bit_counter++;
			}
			else{
				if(counter_time == 2250){
					counter_time = counter_time -2250;
					my_register = my_register<<1;
					my_register = my_register | 0x01;
					bit_counter++;
					
				}
			}
			break;
		case(END):
			Total_time++;
			if(Total_time > 68000){
				State.currentState = IDLE;
				Total_time = 0;
				counter_time = 0;
			}
			break;
		
		default:
			break;
	}
}
