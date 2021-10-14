#include <avr/interrupt.h>
#define RTC_TICK 122
#include "Buzzer.h"
#include "StateDefinitions.h"
#include "BrigthnessMode.h"

/************************************************************************/
/* Local Variables                                                                     */
/************************************************************************/

int32_t volatile counter = 0;
int start_counter=0;
volatile int16_t idle_flag = 0;
volatile int16_t command_counter_flag=0;
volatile int32_t tick_counter = 0;
volatile int16_t total_tick_counter = 0;
volatile int32_t command_register = 0x0000;
volatile int32_t bit_counter = 0;
volatile int16_t i;
volatile int32_t hold_counter = 0;
volatile int16_t hold_flag = 0;
volatile int32_t boost_timer =0;
volatile int32_t boost_total_timer = 0;
volatile int32_t dev_mode_flag = 0;
volatile int32_t dev_mode_timer = 0;
volatile int32_t bright_mode_counter = 0;
volatile int32_t bright_mode_flag = 0;
volatile int64_t bright_level = 0 ;

//Function Prototypes

bool VerifyAddress();
void DecodeCommand();
void disableIR_ISR();
void enableIR_ISR();
void disableRTC();
void enableRTC();
void setState();
void WriteInput();
void GenerateRepeatCode();
/************************************************************************/
/* First 2 bytes are address decimals. They are "129" and "102" in decimal
We need to verify this decimals, because different remotes can send IR signals also.
This function verifies first two bytes. Returns "1" if the address are true, "0" if false.                                                                   */
/************************************************************************/
bool VerifyAddress(){
	if((command_register & 0xFFFF0000) == ADDRESS_bm)
	return true;
	else return false;
}
/************************************************************************/
/* Generates a repeat code. When a key is pressed, we enter to this function and change the current state to hold states.                                                                     */
/************************************************************************/
void GenerateRepeatCode(){
	tick_counter=0;
	hold_flag=1;
	if(InputState.currentState==POWER || InputState.currentState==POWER_HOLD){
		InputState.currentState=POWER_HOLD;
		
	}
	else if(InputState.currentState==INCREMENT || InputState.currentState==INCREMENT_HOLD){
		InputState.currentState=INCREMENT_HOLD;
	}
	else if(InputState.currentState==DECREMENT ||InputState.currentState==DECREMENT_HOLD){
		InputState.currentState=DECREMENT_HOLD;
	}
	else if(InputState.currentState==LIGHT || InputState.currentState==LIGHT_HOLD){
			InputState.currentState=LIGHT_HOLD;
	}
}
/************************************************************************/
/* This functions decodes incoming command. There are 4 different commands which are;
POWER, INCREMENT, DECREMENT and  LIGHT. There can also be REPEAT code which happens when the button 
is held down.Functions changes the current state of InputState instance.                                                                     */
/************************************************************************/
void DecodeCommand(){
	if((command_register&0x0000FFFF)==IR_POWER_bm){
		InputState.currentState = POWER;
		WriteInput();
		setState();
		return;
	}
	else if((command_register&0x0000FFFF)==IR_MINUS_bm){
		InputState.currentState = DECREMENT;
		WriteInput();
		setState();
		return;
	}
	else if ((command_register&0x0000FFFF)==IR_PLUS_bm){
		InputState.currentState = INCREMENT;
		WriteInput();
		setState();
		return;
	}
	else if((command_register&0x0000FFFF)==IR_LIGHT_bm){
		InputState.currentState = LIGHT;
		WriteInput();
		
		return;
	}
	else return;
	
}

/************************************************************************/
/* This function disables interrupt for IR receiver                                                                     */
/************************************************************************/
void disableIR_ISR(){
	PORTA.PIN3CTRL &= 0b00000000;
}
/************************************************************************/
/* This function enables interrupt for IR Receiver                                                                     */
/************************************************************************/
void enableIR_ISR(){
	PORTA.PIN3CTRL |= 0b00000011;
	RTC.CNT = 0;//reset counter
}
/************************************************************************/
/* This function disables the RTC interrupt                                                                     */
/************************************************************************/
void disableRTC(){
	RTC.CNT = 0;//reset counter
	RTC.INTCTRL &= ~RTC_OVF_bm;
	tick_counter = 0;
}
/************************************************************************/
/* This function enables the RTC interrupt.                                                                     */
/************************************************************************/
void enableRTC(){
	RTC.INTCTRL |= RTC_OVF_bm; 
}
/************************************************************************/
/* When we enter the developer mode, a light sequence can be observed. Every second,
the consecutive lights are lit and the lights appear to be moving. Motors are closed in
developer mode.                                                                     */
/************************************************************************/
void DevModeBlink(){
	PORTD.OUT = 0x00; //Close all motors
	switch(DevModeLightState.currentState){
		case(DEV_ONE):
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2);
			DevModeLightState.currentState = DEV_TWO;
		break;
		case(DEV_TWO):
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_2) & ~(1<<LED_3);
			DevModeLightState.currentState = DEV_THREE;
			break;
		case(DEV_THREE):
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_3) & ~(1<<LED_4);
			DevModeLightState.currentState = DEV_FOUR;
			break;
		case(DEV_FOUR):
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_4);
			DevModeLightState.currentState = DEV_ONE;	
			break;
		default:
			DevModeLightState.currentState = DEV_ONE;
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_4);
			break;
	}
}
/************************************************************************/
/* Changes LED and motor behavior according to the current state.                                                                     */
/************************************************************************/
void setState(){	
	switch(State.currentState){
		case(OFF):
			PORTA.OUT = 0xFF; // All LEDs OFF
			PORTD.OUT = 0x00; // All MOTORs OFF
			boost_total_timer = 0;//Coming from BOOST state, reset boost timer.
			break;
		case(ONE):
			//Only LED_1 is on.
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_2);//CLOSE MOTOR_2
			PORTD.OUT |= (1<<MOTOR_RELAY_1);//OPEN MOTOR_1
			break;
		case(TWO):
			//LEDs 1-2 are on.
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_3);//CLOSE MOTOR_3
			PORTD.OUT |= (1<<MOTOR_RELAY_2);//OPEN MOTOR_2
			
			break;
		case(THREE):
			//LEDs 1-2-3 are on.
			boost_total_timer = 0; // Coming from BOOST state, reset boost timer.
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2) & ~(1<<LED_3);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_4);//CLOSE MOTOR_4
			PORTD.OUT |= (1<<MOTOR_RELAY_3);//OPEN MOTOR_3
			
			//
			break;
		case(BOOST):
			//LEDs 1-2-3-4 are on. After 15 min, go to state THREE.
			PORTA.OUT = 0xFF;
			PORTA.OUT &= ~(1<<LED_1) & ~ (1<<LED_2) & ~ (1<<LED_3) & ~(1<<LED_4);
			
			PORTD.OUT |= (1<<MOTOR_RELAY_4);//OPEN MOTOR_4
			break;
		case(DEV_MODE):
				DevModeBlink();
			break;
		case(BRIGHT_ADJ):
				PORTA.OUT = 0xFF; // All LEDs OFF
				PORTD.OUT = 0x00; // All MOTORs OFF
				AdjustBrightness();
			break;	
		default:
			State.currentState = ONE; // In case of errors or out of state machine, go to state one.
			break;
	}
	
}
/************************************************************************/
/* This function changes the State of the system according to the input from the controller                                                                     */
/************************************************************************/
void WriteInput(){
	
	switch(InputState.currentState){
		case(POWER):
			if(State.currentState == OFF){
				State.currentState = ONE;
				BuzzerState.currentState= POWER_ON_SOUND;
				Buzzer();
			}
			else{
				State.currentState = OFF;
				BuzzerState.currentState= POWER_OFF_SOUND;
				Buzzer();
			}
			break;
		case(INCREMENT):
			BuzzerState.currentState= INC_SOUND;
			Buzzer();
			if(State.currentState==ONE){
				State.currentState = TWO;
			}
			else if(State.currentState == TWO){
				State.currentState=THREE;
			}
			else if(State.currentState == THREE) {
				State.currentState = BOOST;
			}
			else if(State.currentState==BRIGHT_ADJ){
				incrementLight();
			}
			break;
		case(DECREMENT):
			BuzzerState.currentState= DEC_SOUND;
			Buzzer();
			if(State.currentState==BOOST){
				State.currentState = THREE;
			}
			else if(State.currentState == THREE){
				State.currentState=TWO;
			}
			else if(State.currentState == TWO) {
				State.currentState = ONE;
			}
			else if(State.currentState==BRIGHT_ADJ){
				decrementLight();
			}
			break;
		case(LIGHT):
			if(LightState.currentState == LIGHT_ON){
				LightState.currentState=LIGHT_OFF;
			}
			else{
				LightState.currentState=LIGHT_ON;
			}
			BuzzerState.currentState= LIGHT_SOUND;
			Buzzer();
			break;
		default:
			break;
	}
}
/************************************************************************/
/* This function reads the Infrared input from the controller and changes the
state of the INPUT_STATE. This function includes the main algorithm to decode the incoming signal.
After going to interrupt when a falling edge is received, we are disabling the IR ISR. We are decoding the signal
according to the NEC protocol.                                                                  */
/************************************************************************/
void IR_Read(){
	//If the signal is HIGH for 9ms, this means we are in START.
	switch(NECState.currentState){
			
		case(COMMAND):
		
			idle_flag=1;
			cli();
			RTC.CNT = 0;
			sei();
			//start timer
			
			disableIR_ISR();
			while(((PORTA.IN & (1<<IR_INPUT)) == 0)&& counter*RTC_TICK <= 9000){
				
			}
			counter = 0;
			cli();
			RTC.CNT = 0;
			sei();
			
			while(!((PORTA.IN & (1<<IR_INPUT)) == 0)&& counter*RTC_TICK <= 4500){
				
			}
			if(counter*RTC_TICK <= 2300){
				//Repeat Case
				counter = 0;
				disableRTC();//reset timer
				enableRTC();
				cli();
				RTC.CNT = 0;
				sei();
				counter = 0;
				while(((PORTA.IN & (1<<IR_INPUT)) == 0)&& counter*RTC_TICK <= 600){
					//end of message
				}
				
				GenerateRepeatCode();
				
				counter=0;
				idle_flag=0;
				cli();
				RTC.CNT = 0;
				sei();
				
				enableIR_ISR();
				break;
			}
			
			//hold_counter = 0; // Reset hold counter if not received repeat signal.
			cli();
			RTC.CNT = 0;
			sei();
			counter = 0;
			idle_flag=0;
			command_counter_flag=1;
			for(i=0;i<33;i++){
				tick_counter=0;
				while(((PORTA.IN & (1<<IR_INPUT)) == 0)&&tick_counter*RTC_TICK<=650){
					//loop until next space
				}
				
				tick_counter=0;
				disableRTC();//reset timer
				enableRTC();
				while(!((PORTA.IN & (1<<IR_INPUT)) == 0)&&tick_counter*RTC_TICK<=1800){
					//loop until next pulse
				}
				if(i==32 && tick_counter*RTC_TICK> 1000){//end of message
					break;
				}
				if( tick_counter*RTC_TICK> 1000)   {                            
					command_register = command_register<<1;   
					command_register |= 0x01;
					tick_counter=tick_counter;
				    }                
				else    {                                         
					command_register = command_register<<1;
					tick_counter=tick_counter;
				}
				
				
				cli();
				RTC.CNT = 0;
				sei();
				
			}
			//wait for end bit. It is a inverse pulse for 500 us
			//If any signal comes in 40ms it is repeat
			//If not go to idle case.
			
			if(VerifyAddress()){
				DecodeCommand();
			}
			
			idle_flag=1;
			counter = 0;
			cli();
			RTC.CNT = 0;
			sei();
			//If we did not received messages for more than 100 ms, it means no repeat. Change flag
			while(!((PORTA.IN & (1<<IR_INPUT)) == 0)&& counter*RTC_TICK <= 50000){
				
			}
			if(counter*RTC_TICK >= 40000){
				hold_flag=0;
				hold_counter = 0;
				if(InputState.currentState==POWER_HOLD){
					InputState.currentState=POWER;
				}
				else if(InputState.currentState==INCREMENT_HOLD){
					InputState.currentState=INCREMENT;
				}
				else if(InputState.currentState==DECREMENT_HOLD){
					InputState.currentState=DECREMENT;
				}
				else if(InputState.currentState==LIGHT_HOLD){
					InputState.currentState=LIGHT;
				}
			}
			idle_flag=0;
			tick_counter=0;
			cli();
			RTC.CNT = 0;
			sei();
			
			enableIR_ISR();
			//disable timer in the end
			break;
		
		case(END):
			counter=0;
			break;
		default:
			break;
	}
}



