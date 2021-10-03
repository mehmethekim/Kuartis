/*
 * Interrupt_test.c
 *
 * Created: 02/10/2021 20:24:38
 * Author :  Mehmet Hekimoðlu
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
volatile uint8_t counter = 0x10; //0b0001 0000;
int cliFlag =0;

ISR(INT0_vect){
	//If 1000 0000 reset to 0001 0000
	//increment
	if(counter>=0x80){
		counter = 0x10;
	}
	else {counter *= 2;}
		
		
		
	
}
ISR(INT1_vect){
		if(counter<=0x10){
			counter = 0x80;
		}
		else {counter /= 2;}
		
}
int main(void)
{
	DDRD &= 0xF3;
	DDRD |= 0xF0; //0b11110000 LED's are output, Interrupts are input
    PORTD = 0x0C; //0b00001100 SET Interrupts to 1. We will use falling edge trigger.
	EICRA |= (0b10<<ISC10) | (0b10<<ISC00); // For interrupt 1, ISC11 and ISC10 is set to 1-0, For interrupt 0 ISC01 and ISC00 is set to 1-0 (For falling edge)
	EIMSK = 0x03; //0b00000011 Enable interrupt SREG enabled
	 // enable global interrupts
	sei();
	
    while (1) 
    {
		
		PORTD &= 0x0F;
		PORTD |= counter; 
		
	
		
		
		
		
    }
}

