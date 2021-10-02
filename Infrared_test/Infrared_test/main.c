/*
 * Infrared_test.c
 *
 * Created: 02/10/2021 17:06:46
 * Author : mehme
 */ 
#include <stdio.h>
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

typedef enum{
	zero = 0,
	one = 1,
	two = 2,
	three = 3,
	four = 4
	}STATE;
typedef struct STATE_INFO {
	STATE currentState;
	}STATE_INFO;
STATE_INFO State;

#define zero_mask  0b00000000;
#define one_mask   0b00100000;//only PORTB5 open
#define two_mask   0b00110000;
#define three_mask 0b00111000;
#define four_mask  0b00111100; 

int main(void)
{
	
	State.currentState = zero;
	DDRB = 0b00111100;
	PORTB = 0;
    while (1) 
    {
		switch(State.currentState){
			case(zero):
				PORTB = zero_mask;
				_delay_ms(500);
				State.currentState = one;
				break;
			case(one):
				PORTB = one_mask;
				_delay_ms(500);
				State.currentState = two;
				break;
			case(two):
				PORTB = two_mask;
				_delay_ms(500);
				State.currentState = three;
				break;
			case(three):
				PORTB = three_mask;
				_delay_ms(500);
				State.currentState = four;
				break;
			case(four):
				PORTB = four_mask;
				_delay_ms(500);
				State.currentState = zero;
				break;
			default:
				_delay_ms(500);
				break;
		}
    }
}

