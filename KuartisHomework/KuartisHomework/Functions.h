//Global Variables


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


//Functions
/************************************************************************/
/* Initializes LEDs and MOTOR ports. Sets the initial state to START State                                                                     */
/************************************************************************/
void Initialize(){
	//Initialize LEDs
	PORTA.DIR = (1<<LED_1) | (1<<LED_2) | (1<<LED_3) | (1<<LED_4) ;
	PORTA.OUT = 0xFF; // Close All LEDs
	
	//Initialize Motors
	
	PORTD.DIR = (1<<MOTOR_RELAY_1) | (1<<MOTOR_RELAY_2)  |(1<<MOTOR_RELAY_3) |(1<<MOTOR_RELAY_4);
	PORTD.OUT = 0x00; //Close all motors
	
	//Initial state
	State.currentState = OFF;
	LightState.currentState = OFF;
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
//Early Read function for debug purposes
void IR_Read_Debug(){
}

void Test_One(){
	
	//Press power button
	InputState.currentState = POWER;
	WriteInput();
	setState(); // 1
	
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); // 2
	
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); // 3
	
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //2
	
	//Press Light Button
	InputState.currentState = LIGHT;
	WriteInput();
	setState();
	
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); //3
	
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); //4
	
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); //4
	
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //3
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //2
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //1
	
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //1
	
	//Press power button
	InputState.currentState = POWER;
	WriteInput();
	setState(); // OFF	
}