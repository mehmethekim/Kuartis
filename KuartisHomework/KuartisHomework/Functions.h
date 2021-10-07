#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#define RTC_EXAMPLE_PERIOD            (511)
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
volatile int16_t tick_counter = 0;
volatile int16_t total_tick_counter = 0;
volatile int32_t command_register = 0x0000;
void RTC_init(void)
{
	uint8_t temp;
	
	/* Initialize 32.768kHz Oscillator: */
	/* Disable oscillator: */
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_ENABLE_bm;
	/* Writing to protected register */
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	
	while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
	{
		; /* Wait until XOSC32KS becomes 0 */
	}
	
	/* SEL = 0 (Use External Crystal): */
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_SEL_bm;
	/* Writing to protected register */
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	
	/* Enable oscillator: */
	temp = CLKCTRL.XOSC32KCTRLA;
	temp |= CLKCTRL_ENABLE_bm;
	/* Writing to protected register */
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	
	/* Initialize RTC: */
	while (RTC.STATUS > 0)
	{
		; /* Wait for all register to be synchronized */
	}

	/* Set period */
	RTC.PER = RTC_EXAMPLE_PERIOD;

	/* 32.768kHz External Crystal Oscillator (XOSC32K) */
	RTC.CLKSEL = RTC_CLKSEL_TOSC32K_gc;

	/* Run in debug: enabled */
	RTC.DBGCTRL |= RTC_DBGRUN_bm;
	
	RTC.CTRLA = RTC_PRESCALER_DIV32_gc  /* 32 */
	| RTC_RTCEN_bm            /* Enable: enabled */
	| RTC_RUNSTDBY_bm;        /* Run In Standby: enabled */
	
	/* Enable Overflow Interrupt */
	RTC.INTCTRL |= RTC_OVF_bm;
}
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
	RTC.PER = 32768; 
	RTC.INTCTRL |= RTC_OVF_bm; 
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc|RTC_RTCEN_bm | RTC_RUNSTDBY_bm;   
	//OVF set 1
	
	RTC.CMP = 0x8000;
	
	

	
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
		case(IDLE):
			idle_flag=1;
		
			break;
		case(START):
		
			break;
			/*
		case(ADDRESS):
			
			break;
		case(ADDRESS_INV):
		
			break;
			*/
			
		case(COMMAND):
			
			break;
		
		case(END):
		
			break;
		case(REPEAT):
			
			break;
		default:
			break;
	}
}
