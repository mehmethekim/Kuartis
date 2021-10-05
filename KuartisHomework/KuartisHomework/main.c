/*
 * KuartisHomework.c
 *
 * Created: 05/10/2021 15:59:25
 * Author : Mehmet Hekimoðlu
 */ 

#include <avr/io.h>
#include "PortDefinitions.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

int main(void)
{
	// LED TEST RUN
	PORTA.DIR = (1<<LED_1) | (1<<LED_2) | (1<<LED_3) | (1<<LED_4) ;
	PORTA.OUT = 0b11111111;
	PORTA.OUT = 0b11011000; // Set all register to 1. 
	
	
	//PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2) & ~(1<<LED_3) & ~(1<<LED_4); // light all  -> 0b11011000
	//PORTA.OUT |= (1<<LED_1)  | (1<<LED_3); // close 1 and 3 0b00000001 OR 0b00000010 0b00000011
	PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_3);  // light all  1 3 -> 0b11111011 AND 0b11011111->0b11011011    
	
	
	
	
    while (1) 
    {
		
		
    }
}

