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
		if((hold_counter*RTC_TICK>=2*ONE_SEC) && InputState.currentState==POWER_HOLD){//After 4 sec power button press, indicate with buzzer and wait for 2 sec
			dev_mode_flag=1;
			BuzzerState.currentState = DEV_INIT_SOUND;
			Buzzer();
			
			
		}
		if((hold_counter*RTC_TICK>=2*ONE_SEC)&&InputState.currentState== LIGHT_HOLD&&dev_mode_flag==0){
			State.currentState = BRIGHT_ADJ;
			BuzzerState.currentState = LIGHT_ADJ_SOUND;
			Buzzer();
			setState();
		}
		if((hold_counter*RTC_TICK>=1*ONE_SEC)&&InputState.currentState== LIGHT_HOLD&&dev_mode_flag==1){
			State.currentState = DEV_MODE;
			BuzzerState.currentState = DEV_MODE_SOUND;
			Buzzer();
			dev_mode_flag=0;
			//Close all lights at first;
			setState();
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

