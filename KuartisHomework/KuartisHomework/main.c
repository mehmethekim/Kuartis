/*
 * KuartisHomework.c
 *
 * Created: 05/10/2021 15:59:25
 * Author : Mehmet Hekimoðlu
 */ 

#include <avr/io.h>
#include "PortDefinitions.h"
#include "Functions.h"
#include "StateDefinitions.h"
#include "tests.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#ifndef F_CPU
#define F_CPU 20000000UL // 20MHz Clock
#endif


//Can hold a timer for blink at boost mode. To tick at 1 sec time
//also, can hold the blink timer in a integer, timer can tick every 1 us, every 1 000 000 tick-> toggle led 4;
//can hold an integer for 15 min-> 15*60 sec -> 900 max, interrput at 900
//After IR Repeat is received,need to hold that value,

//+/- light adjustment
//Hold that value inside of the chip, maybe write it on EEPROM or ROM


//Global Variables
int volatile state = 0;
int volatile debug_flag = 0;
int main(void)
{
	CCP = 0xD8;
	CLKCTRL.MCLKLOCK =0x00;
	CCP = 0xD8;
    CLKCTRL.MCLKCTRLB = 0;
	CCP = 0xD8;
	CLKCTRL.MCLKLOCK =0x01;
	/*
	CCP = 0xD8;
	CLKCTRL.MCLKLOCK =0x00;
	CCP = 0xD8;
	CLKCTRL.MCLKCTRLA = 0x00;
	CCP = 0xD8;
	CLKCTRL.MCLKCTRLB = 0x13;
	CCP = 0xD8;
	CLKCTRL.MCLKLOCK =0x01;
	*/
	PORTD.DIR = (1<<LED_ON_OFF);
	PORTD.OUT = 0x00; // Close All LEDs
	Initialize();
	
	//Test_One();
	
    while (1) 
    {
		
    }
}
//Incoming signal, falling edge triggered.

/************************************************************************/
/* Timer interrupt increases a counter every 1ms.                                                                     */
/************************************************************************/

ISR(PORTA_PORT_vect){
	
	IR_Read();
	disableIR_ISR();
	 //  disable interrupt 
}
ISR(RTC_CNT_vect){
	 RTC.INTFLAGS = RTC_OVF_bm;
	if(idle_flag == 1){
		counter++;
		if(counter >= 4){
			counter = 0;
		}
	}
	else{
		counter = 0;
	}
	
}

/*
ISR(TCA0_OVF_vect){
	

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
	//IDLE to START case
	if(idle_flag==1){
		
		
		counter++;
		if(counter>=20000){
			counter=0;
		}
		
		
		if((PORTA.IN & (1<<IR_INPUT)) == 0){
			counter++;
			if(counter>=8){
				NECState.currentState = START;
				idle_flag=0;
				TCA0.SINGLE.INTCTRL &= ~TCA_SINGLE_OVF_bm;
				IR_Read();
				counter = 0;
			}
		}
		else{
			//start_counter = 0;//if signal is less than 9ms, reset counter and flag;
			//idle_flag=0;
		}
		
	}
	//AFTER START
	if(NECState.currentState == COMMAND){
		
		if(command_counter_flag==1){
			IR_Read();
			tick_counter++;
			total_tick_counter++;
			if(total_tick_counter>= 54000 ){
				command_counter_flag = 0;
				command_register = 0x0000;
			}
			
		}
		
	}
	//TCA0.SINGLE.CNT = 0;

}
	*/

