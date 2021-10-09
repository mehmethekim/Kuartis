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
	CPUINT.LVL1VEC = RTC_CNT_vect_num;
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
	NECState.currentState = COMMAND;
	IR_Read(); 
}
ISR(RTC_CNT_vect){
	 RTC.INTFLAGS = RTC_OVF_bm;
	 
	if(idle_flag == 1){
		counter++;
		}
	if(command_counter_flag==1){
		tick_counter++;
		total_tick_counter++;	
		}
	if(State.currentState==BOOST){
		boost_timer++;
		boost_total_timer++;
		if(boost_timer*RTC_TICK >= ONE_SEC){ // 1sec
			PORTA.OUT ^= (1<<LED_4);
			boost_timer=0;
		}
		if(boost_total_timer*RTC_TICK>=BOOST_MAX_30SEC){
			State.currentState = THREE;
			setState();
		}
	}
	if(hold_flag==1){
		hold_counter++;
		if((hold_counter*RTC_TICK>=4*ONE_SEC)&&InputState.currentState== POWER_HOLD){//After 4 sec power button press, indicate with buzzer and wait for 2 sec 
				dev_mode_flag=1;
				
				
				setState();
		}
		if((hold_counter*RTC_TICK>=4*ONE_SEC)&&InputState.currentState== LIGHT_HOLD){
			State.currentState = BRIGHT_ADJ;
			setState();
		}
		if((hold_counter*RTC_TICK>=2*ONE_SEC)&&InputState.currentState== LIGHT_HOLD&&dev_mode_flag==1){
			State.currentState = DEV_MODE;
		}
	}
	if(BuzzerFlag==1){
		BuzzerCounter++;
		PORTD.OUT |= (1<<BUZZER); 
		Buzzer();
		
		if(BuzzerCounter*RTC_TICK>=ONE_SEC/10){
			BuzzerFlag=0;
			BuzzerCounter=0;
			Pitch_flag=0;
			PORTD.OUT &= ~(1<<BUZZER); 
		}
		if(BuzzerCounter*RTC_TICK>=ONE_SEC/20){
			Pitch_flag=1;
		}
	}
}
