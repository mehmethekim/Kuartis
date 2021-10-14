/* This enumerator holds the states of the hood appliance. Can be incremented
or decremented by the controller.                                                                    */
/************************************************************************/
typedef enum{
	OFF,
	ONE,
	TWO,
	THREE,
	BOOST,
	DEV_MODE,
	BRIGHT_ADJ
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
	LIGHT_HOLD,
	INITIAL_STATE
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
/* This enumerator holds the states for sweeping LEDs in developer mode.
Two consecutive LEDs will be lit and they will move every one sec.                                                                    */
/************************************************************************/
typedef enum{
	DEV_ONE,
	DEV_TWO,
	DEV_THREE,
	DEV_FOUR
}DEV_MODE_LIGHT_STATE;
/************************************************************************/
/* This enumerator holds the states for brightness level. Brightness level
is kept the same even if the device is turned off. It is controlled by a PWM output.                                                                     */
/************************************************************************/
typedef enum{
	BRIGHT_ONE,
	BRIGHT_TWO,
	BRIGHT_THREE,
	BRIGHT_FOUR,
	BRIGHT_FIVE,
	BRIGHT_SIX,
	BRIGHT_SEVEN,
	BRIGHT_EIGHT,
	BRIGHT_NINE,
	BRIGHT_TEN
	}BRIGHT_MODE_STATE;
/************************************************************************/
/* This structure holds the current state of the brightness level.                                                                     */
/************************************************************************/
typedef struct BRIGHT_MODE_STATE_INFO{
	BRIGHT_MODE_STATE currentState;
}BRIGHT_MODE_STATE_INFO;
/************************************************************************/
/*This structure hold the current state of LEDs in developer mode.                                                                   */
/************************************************************************/
typedef struct DEV_MODE_LIGHT_STATE_INFO{
	DEV_MODE_LIGHT_STATE currentState;
}DEV_MODE_LIGHT_STATE_INFO;
/************************************************************************/
/* Enumerator for boolean type. Avoid including <stdbool> for simplicity.                                                                    */
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

// Global State Variables.
IR_STATE_INFO NECState;
INPUT_STATE_INFO InputState;
STATE_INFO State;
LIGHT_STATE_INFO LightState;
REPEAT_STATE_INFO RepeatState;
DEV_MODE_LIGHT_STATE_INFO DevModeLightState;
BRIGHT_MODE_STATE_INFO BrightModeState;
BRIGHT_MODE_STATE_INFO PreviousBrightModeState;

