/************************************************************************/
/* This function simulates the current brightness level using 4 LEDs on the hood.
To simulate we are counting to "10" in binary.                                                                  */
/************************************************************************/
void AdjustBrightness(){
	switch(BrightModeState.currentState){
		case(BRIGHT_ONE):
			PORTA.OUT &= ~(1<<LED_1);
		break;
		case(BRIGHT_TWO):
			PORTA.OUT &=  ~(1<<LED_2);
		break;
		case(BRIGHT_THREE):
			PORTA.OUT &= ~(1<<LED_1) & ~(1<<LED_2);
		break;
		case(BRIGHT_FOUR):
			PORTA.OUT &= ~(1<<LED_3) ;
		break;
		case(BRIGHT_FIVE):
			PORTA.OUT &= ~(1<<LED_3) & ~(1<<LED_1);
		break;
		case(BRIGHT_SIX):
			PORTA.OUT &= ~(1<<LED_3) & ~(1<<LED_2);
		break;
		case(BRIGHT_SEVEN):
			PORTA.OUT &= ~(1<<LED_3) & ~(1<<LED_2) & ~(1<<LED_1);
		break;
		case(BRIGHT_EIGHT):
			PORTA.OUT &= ~(1<<LED_4);
		break;
		case(BRIGHT_NINE):
			PORTA.OUT &= ~(1<<LED_4) & ~(1<<LED_1);
		break;
		case(BRIGHT_TEN):
			PORTA.OUT &= ~(1<<LED_4) & ~(1<<LED_2);
		break;
		default:
			PORTA.OUT = 0xFF; // All LEDs OFF
			PORTD.OUT = 0x00; // All MOTORs OFF
		break;
		
		
	}
}
/************************************************************************/
/* This function increases the brightness level by 1. If we are at level 10,
the level increases no more.                                                                     */
/************************************************************************/
void incrementLight(){
	switch(BrightModeState.currentState){
		case(BRIGHT_ONE):
			BrightModeState.currentState = BRIGHT_TWO;
		break;
		case(BRIGHT_TWO):
			BrightModeState.currentState = BRIGHT_THREE;
		break;
		case(BRIGHT_THREE):
			BrightModeState.currentState = BRIGHT_FOUR;
		break;
		case(BRIGHT_FOUR):
			BrightModeState.currentState = BRIGHT_FIVE;
		break;
		case(BRIGHT_FIVE):
			BrightModeState.currentState = BRIGHT_SIX;
		break;
		case(BRIGHT_SIX):
			BrightModeState.currentState = BRIGHT_SEVEN;
		break;
		case(BRIGHT_SEVEN):
			BrightModeState.currentState = BRIGHT_EIGHT;
		break;
		case(BRIGHT_EIGHT):
			BrightModeState.currentState = BRIGHT_NINE;
		break;
		case(BRIGHT_NINE):
			BrightModeState.currentState = BRIGHT_TEN;
		break;
		case(BRIGHT_TEN):
		//Cannot increment more
		break;
		default:
		break;
				
	}
}
/************************************************************************/
/* This function decreases the brightness level by 1. If we are at level 1,
the level decreases no more.                                                                     */
/************************************************************************/
void decrementLight(){
	switch(BrightModeState.currentState){
		case(BRIGHT_ONE):
		//Cannot decrement more
		break;
		case(BRIGHT_TWO):
		BrightModeState.currentState = BRIGHT_ONE;
		break;
		case(BRIGHT_THREE):
		BrightModeState.currentState = BRIGHT_TWO;
		break;
		case(BRIGHT_FOUR):
		BrightModeState.currentState = BRIGHT_THREE;
		break;
		case(BRIGHT_FIVE):
		BrightModeState.currentState = BRIGHT_FOUR;
		break;
		case(BRIGHT_SIX):
		BrightModeState.currentState = BRIGHT_FIVE;
		break;
		case(BRIGHT_SEVEN):
		BrightModeState.currentState = BRIGHT_SIX;
		break;
		case(BRIGHT_EIGHT):
		BrightModeState.currentState = BRIGHT_SEVEN;
		break;
		case(BRIGHT_NINE):
		BrightModeState.currentState = BRIGHT_EIGHT;
		break;
		case(BRIGHT_TEN):
		BrightModeState.currentState = BRIGHT_NINE;
		break;
		default:
		break;
	}
}
