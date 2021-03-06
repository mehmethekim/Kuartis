/************************************************************************/
/* this interrupt function triggers when the infrared signal is received by a falling edge.
After receiving the message, this interrupt is disabled for a short period of time to process incoming data,
as there are falling edges in the signal and we should not trigger the interrupt.                                                                     */
/************************************************************************/
ISR(PORTA_PORT_vect){
	NECState.currentState = COMMAND;
	IR_Read();
}
/************************************************************************/
/* This interrupt is Real Time Clock (RTC) interrupt. We are given it high priority because no matter what,
we want trigger every 0.122 us. According to the different states, certain flags are opened and closed to 
change different counter variables.                                                                     */
/************************************************************************/
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
	// If a key is kept pressed, hold_flag = 1. When released, we are setting it to 0.
	if(hold_flag==1){
		hold_counter++;
		if((hold_counter*RTC_TICK>=2*ONE_SEC) && InputState.currentState==POWER_HOLD){//After 4 sec power button press, indicate with buzzer.
			dev_mode_flag=1;
			BuzzerState.currentState = DEV_INIT_SOUND;
			Buzzer();
			hold_counter=0;
			
		}
		// When light button is held for 4 sec, we are going inside and setting the current state to Brightness Adjustment.
		if((hold_counter*RTC_TICK>=2*ONE_SEC)&&InputState.currentState== LIGHT_HOLD&&dev_mode_flag==0&&bright_mode_flag==0){
			State.currentState = BRIGHT_ADJ;
			bright_mode_flag=1;
			BuzzerState.currentState = LIGHT_ADJ_SOUND;
			Buzzer();
			setState();
			hold_counter=0;
		}
		// If we are in Brightness Adjustment mode, if light button is pressed for 4 sec, write the value to EEPROM and go to state ONE.
		if((hold_counter*RTC_TICK>=2*ONE_SEC)&&InputState.currentState== LIGHT_HOLD&&dev_mode_flag==0&&bright_mode_flag==1){
			State.currentState = ONE;
			setState();
			bright_mode_flag=0;
			BuzzerState.currentState = LIGHT_ADJ_SOUND;
			Buzzer();
			hold_counter=0;
			EEPROM_write(BrightModeState);
			bright_level=EEPROM_read();
			bright_level++;
		}
		// After power button is pressed for 4 sec and light button for 2 sec, change state to developper mode.
		if((hold_counter*RTC_TICK>=1*ONE_SEC)&&InputState.currentState== LIGHT_HOLD&&dev_mode_flag==1){
			State.currentState = DEV_MODE;
			BuzzerState.currentState = DEV_MODE_SOUND;
			Buzzer();
			dev_mode_flag=0;
			//Close all lights at first;
			setState();
		}
	}
	// When the flag is set, we are calling Buzzer function and count a variable for different frequencies.
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
	// If we are in developper mode, change light scheme every second.
	if(State.currentState==DEV_MODE){
		dev_mode_timer++;
		if(dev_mode_timer*RTC_TICK>=ONE_SEC){
			dev_mode_timer=0;
			setState();
		}
	}
	if(State.currentState==BRIGHT_ADJ){
		bright_mode_counter++;
		if(bright_mode_counter==10){
			bright_mode_counter = 0;
		}
	}
}

/************************************************************************/
/* This interrupt function is triggered when we want to send data using USART.                                                                     */
/************************************************************************/
ISR(USART0_TXC_vect){
	
}
/************************************************************************/
/* This interrupt function is triggered when we receive a message from USART interface.                                                                     */
/************************************************************************/
ISR(USART0_RXC_vect){
	
}
