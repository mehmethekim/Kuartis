/*
 * Timer_test.c
 *
 * Created: 03/10/2021 13:51:59
 * Author : Mehmet Hekimoðlu
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int timeVar = 0;
int main(void)
{
    TCCR0A = (1<<WGM01);
	OCR0A = 250;
	TCCR0B = (1<<CS01)| (1<<CS00); // 64 prescalar. to count 1ms each clock
	TIMSK0 = (1<<OCIE0A);
	DDRB = (1<<PORTB5); // set port 5 as an output
	
	
	
	sei();
	
    while (1) 
    {
    }
}
//every ms go to timer
//every sec toggle
ISR(TIMER0_COMPA_vect){
	timeVar++;
	if(timeVar > 1000){
		PORTB ^= (1<<PORTB5);
		timeVar = 0;
	}
}