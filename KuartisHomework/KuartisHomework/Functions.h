#include <avr/interrupt.h>
#define RTC_TICK 122
//Functions
/************************************************************************/
/* Initializes LEDs and MOTOR ports. Sets the initial state to START State                                                                     */
/************************************************************************/
/************************************************************************/
/* This enumerator holds the states of the hood appliance. Can be incremented
or decremented by the controller.                                                                    */
/************************************************************************/
typedef enum{
	OFF,
	ONE,
	TWO,
	THREE,
	BOOST
}STATE;
/************************************************************************/
/* This enumerator holds the state of the light. Either on/off. Controlled by the light button.                                                                     */
/************************************************************************/
typedef enum{
	LIGHT_ON,
	LIGHT_OFF
}LIGHT_STATE;
/************************************************************************/
/* This enumerator holds the input from controller.                                                                     */
/************************************************************************/
typedef enum{
	POWER,
	POWER_HOLD,
	INCREMENT,
	INCREMENT_HOLD,
	DECREMENT,
	DECREMENT_HOLD,
	LIGHT,
	LIGHT_HOLD
}INPUT_STATE;
/************************************************************************/
/* This enumerator holds the states of the infrared NEC protocol.                                                                     */
/************************************************************************/
typedef enum{
	IDLE,
	START,
	//ADDRESS,
	//ADDRESS_INV,
	COMMAND,//32 bit information
	//COMMAND_INV,
	END,
	REPEAT
	}IR_STATE;
/************************************************************************/
/* This enumerator holds the repeated signals state in NEC protocol.                                                                     */
/************************************************************************/
typedef enum{
	NO_REPEAT,
	POWER_REPEAT,
	INC_REPEAT,
	DEC_REPEAT,
	LIGHT_REPEAT
	}REPEAT_STATE;
/************************************************************************/
/* Enumerator for boolean type                                                                     */
/************************************************************************/
typedef enum {false, true} bool;
/************************************************************************/
/* This struct holds the current state of the light.                                                                     */
/************************************************************************/
typedef struct LIGHT_STATE_INFO{
	LIGHT_STATE currentState;
}LIGHT_STATE_INFO;
/************************************************************************/
/* Holds the current state of the hood appliance.                                                                     */
/************************************************************************/
typedef struct STATE_INFO{
	STATE currentState;
	
}STATE_INFO;
/************************************************************************/
/* Holds the state of input from controller                                                                     */
/************************************************************************/
typedef struct INPUT_STATE_INFO{
	INPUT_STATE currentState;
}INPUT_STATE_INFO;
/************************************************************************/
/* Holds the current state of Infrared NEC protocol                                                                     */
/************************************************************************/
typedef struct IR_STATE_INFO{
	IR_STATE currentState;
	}IR_STATE_INFO;
/************************************************************************/
/* Holds the current state of repeated signal.                                                                     */
/************************************************************************/
typedef struct REPEAT_STATE_INFO{
	REPEAT_STATE currentState;
}REPEAT_STATE_INFO;
/************************************************************************/
/* Local State Variables                                                                     */
/************************************************************************/
IR_STATE_INFO NECState;
INPUT_STATE_INFO InputState;
STATE_INFO State;
LIGHT_STATE_INFO LightState;
REPEAT_STATE_INFO RepeatState;
int32_t volatile counter = 0;
int start_counter=0;
volatile int16_t idle_flag = 0;
volatile int16_t command_counter_flag=0;
volatile int32_t tick_counter = 0;
volatile int16_t total_tick_counter = 0;
volatile int32_t command_register = 0x0000;
volatile int32_t bit_counter = 0;
volatile int16_t i;
volatile int16_t hold_counter = 0;
volatile int16_t hold_flag = 0;
volatile int32_t boost_timer =0;

//Function Prototypes
void Initialize();
bool VerifyAddress();
void DecodeCommand();
void disableIR_ISR();
void enableIR_ISR();
void disableRTC();
void enableRTC();
void setState();
void WriteInput();
void GenerateRepeatCode();
void Initialize(){
	//Initialize LEDs
	sei();
	PORTA.DIR = (1<<LED_1) | (1<<LED_2) | (1<<LED_3) | (1<<LED_4) ;
	PORTA.OUT = 0xFF; // Close All LEDs
	
	//Initialize Motors
	
	PORTD.DIR = (1<<MOTOR_RELAY_1) | (1<<MOTOR_RELAY_2)  |(1<<MOTOR_RELAY_3) |(1<<MOTOR_RELAY_4);
	PORTD.OUT = 0x00; //Close all motors
	//Initialize Buzzer
	PORTD.DIR |= (1<<BUZZER);
	PORTD.OUT &= ~(1<<BUZZER);
	//Initialize IR Receiver
	PORTA.DIR &= ~(1<<IR_INPUT);
	
	PORTA.PIN3CTRL |= 0b00000011; //SET ISC to 0x3 to have falling edge trigger. ISC is last 3 bits. Set to 011.
	SREG |= (1<<GLOBAL_INT_ENABLE); //Enable Interrupts
	 /* Enable Global Interrupts */
	 sei();

	//Initial state
	State.currentState = OFF;
	LightState.currentState = OFF;
	NECState.currentState = IDLE;
	
	//RTC initialize
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;//32.768 kHz RTC
	while (RTC.STATUS > 0); 
	RTC.PER = 4;  // 122 us per tick
	RTC.INTCTRL |= RTC_OVF_bm; 
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc|RTC_RTCEN_bm | RTC_RUNSTDBY_bm;   
}
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
void GenerateRepeatCode(){
	tick_counter=0;
	hold_counter++;
	if(InputState.currentState==POWER || InputState.currentState==POWER_HOLD){
		InputState.currentState=POWER_HOLD;
		
	}
	else if(InputState.currentState==INCREMENT || InputState.currentState==INCREMENT_HOLD){
		
	}
	else if(InputState.currentState==DECREMENT ||InputState.currentState==DECREMENT_HOLD){
		
	}
	else if(InputState.currentState==LIGHT || InputState.currentState==LIGHT_HOLD){
		
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
		setState();
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
void disableRTC(){
	RTC.CNT = 0;//reset counter
	RTC.INTCTRL &= ~RTC_OVF_bm;
	tick_counter = 0;
}
void enableRTC(){
	RTC.INTCTRL |= RTC_OVF_bm;
	
	 
}
void setState(){
	
	switch(State.currentState){
		case(OFF):
			PORTA.OUT = 0xFF; // All LEDs OFF
			PORTD.OUT = 0x00; // All MOTORs OFF
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
			}
			else{
				State.currentState = OFF;
			}
			break;
		case(INCREMENT):
			if(State.currentState==ONE){
				State.currentState = TWO;
			}
			else if(State.currentState == TWO){
				State.currentState=THREE;
			}
			else if(State.currentState == THREE) {
				State.currentState = BOOST;
			}
			break;
		case(DECREMENT):
			if(State.currentState==BOOST){
				State.currentState = THREE;
			}
			else if(State.currentState == THREE){
				State.currentState=TWO;
			}
			else if(State.currentState == TWO) {
				State.currentState = ONE;
			}
			break;
		case(LIGHT):
			if(LightState.currentState == LIGHT_ON){
				LightState.currentState=LIGHT_OFF;
			}
			else{
				LightState.currentState=LIGHT_ON;
			}
			break;
		default:
			break;
	}
}
/************************************************************************/
/* This function reads the Infrared input from the controller and changes the
state of the INPUT_STATE                                                                    */
/************************************************************************/
//IR Read function to decode incoming signal.
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
			//wait for 30ms to process and reset,
			while(!((PORTA.IN & (1<<IR_INPUT)) == 0)&& counter*RTC_TICK <= 20000){
				
			}
			//reset the process
			//wait for 40ms to read repeat
			/*
			tick_counter=0;
			cli();
			disableRTC();//reset timer
			enableRTC();
			RTC.CNT = 0;
			sei();
			*/
			/*
			while(!((PORTA.IN & (1<<IR_INPUT)) == 0)&&tick_counter*RTC_TICK<=80000){
				//next pulse is longer than 80 ms means no repeat
			}
			
			command_counter_flag=0;
			if((tick_counter*RTC_TICK) <= 50000){
				GenerateRepeatCode();
				
				tick_counter=0;
			}
			else{ 
			}*/
			
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
		case(REPEAT):
			
			break;
		default:
			break;
	}
}



