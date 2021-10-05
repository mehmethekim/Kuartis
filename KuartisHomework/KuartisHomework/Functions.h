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
	ON,
	OFF
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

INPUT_STATE_INFO InputState;
STATE_INFO State;
LIGHT_STATE_INFO LightState;

/************************************************************************/
/* Initializes LEDs and MOTOR ports. Sets the initial state to START State                                                                     */
/************************************************************************/
void Initialize(){
	//Initialize LEDs
	PORTA.DIR = (1<<LED_1) | (1<<LED_2) | (1<<LED_3) | (1<<LED_4) ;
	PORTA.OUT = 0b11111111; // Close All LEDs
	
	//Initialize Motors
	
	PORTD.DIR = (1<<MOTOR_RELAY_1) | (1<<MOTOR_RELAY_2)  |(1<<MOTOR_RELAY_3) |(1<<MOTOR_RELAY_4);
	PORTD.OUT = 0; //Close all motors
	
	//Initial state
	State.currentState = OFF;
	LightState.currentState = OFF;
}


void setState(){
	
	switch(State.currentState){
		case(OFF):
		
			break;
		case(ONE):
			//Only LED_1 is on.
			PORTA.OUT = 1;
			PORTA.OUT &= ~(1<<LED_1);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_2);//CLOSE MOTOR_2
			PORTD.OUT |= (1<<MOTOR_RELAY_1);//OPEN MOTOR_1
			break;
		case(TWO):
			//LEDs 1-2 are on.
			PORTA.OUT = 1;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_3);//CLOSE MOTOR_3
			PORTD.OUT |= (1<<MOTOR_RELAY_2);//OPEN MOTOR_2
			PORTD 
			break;
		case(THREE):
			//LEDs 1-2-3 are on.
			PORTA.OUT = 1;
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2) & ~(1<<LED_3);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_3);//CLOSE MOTOR_3
			PORTD.OUT |= (1<<MOTOR_RELAY_2);//OPEN MOTOR_2
			//
			break;
		case(BOOST):
			//LEDs 1-2-3-4 are on. After 15 min, go to state THREE.
			PORTA.OUT = 1;
			PORTA.OUT &= ~(1<<LED_1) & ~ (1<<LED_2) & ~ (1<<LED_3);
			
			PORTD.OUT &= ~(0<<MOTOR_RELAY_3);//CLOSE MOTOR_3
			PORTD.OUT |= (1<<MOTOR_RELAY_2);//OPEN MOTOR_2
			break;
		default:
			State.currentState = ONE; // In case of errors or out of state machine, go to state one.
			break;
	}
	
}
void WriteInput(){
	
	switch(INPUT_STATE_INFO.currentState){
		case(POWER):
			if(STATE_INFO.currentState == OFF){
				STATE_INFO.currentState = ONE;
			}
			else{
				STATE_INFO.currentState = OFF;
			}
			break;
		case(INCREMENT):
			if(STATE_INFO.currentState==ONE){
				STATE_INFO.currentState = TWO;
			}
			else if(STATE_INFO.currentState == TWO){
				STATE_INFO.currentState=THREE;
			}
			else if(STATE_INFO.currentState == THREE) {
				STATE_INFO.currentState = BOOST;
			}
			break;
		case(DECREMENT):
			if(STATE_INFO.currentState==BOOST){
				STATE_INFO.currentState = THREE;
			}
			else if(STATE_INFO.currentState == THREE){
				STATE_INFO.currentState=TWO;
			}
			else if(STATE_INFO.currentState == TWO) {
				STATE_INFO.currentState = ONE;
			}
			break;
		case(LIGHT):
			if(LIGHT_STATE_INFO.currentState == ON){
				LIGHT_STATE_INFO.currentState=OFF;
			}
			else{
				LIGHT_STATE_INFO.currentState=ON;
			}
			break;
	}
}
void IR_Read(){
	
}