/************************************************************************/
/* Initializes the main clock to 20MHz. We are using 0xD8 key to open MCLKLOCK,MCLKCTRLB.                                                                     */
/************************************************************************/
void init_CLK(){
	CCP = 0xD8;
	CLKCTRL.MCLKLOCK =0x00;
	CCP = 0xD8;
	CLKCTRL.MCLKCTRLB = 0;
	CCP = 0xD8;
	CLKCTRL.MCLKLOCK =0x01;
}
/************************************************************************/
/* Initializes four LEDs on the hood.                                                                     */
/************************************************************************/
void init_LED(){
	//Initialize LEDs
	PORTA.DIR = (1<<LED_1) | (1<<LED_2) | (1<<LED_3) | (1<<LED_4) ;
	PORTA.OUT = 0xFF; // Close All LEDs
}
/************************************************************************/
/* Initializes the motor relays on the hood.                                                                     */
/************************************************************************/
void init_Motor(){
	//Initialize Motors
	PORTD.DIR = (1<<MOTOR_RELAY_1) | (1<<MOTOR_RELAY_2)  |(1<<MOTOR_RELAY_3) |(1<<MOTOR_RELAY_4);
	PORTD.OUT = 0x00; //Close all motors
}
/************************************************************************/
/* Initializes the buzzer.                                                                     */
/************************************************************************/
void init_Buzzer(){
		PORTD.DIR |= (1<<BUZZER);
		PORTD.OUT &= ~(1<<BUZZER);
}
/************************************************************************/
/* Initializes the Infrared Receiver. Interrupt is set to trigger at falling edge.                                                                     */
/************************************************************************/
void init_IR(){
	//Initialize IR Receiver
	PORTA.DIR &= ~(1<<IR_INPUT);	
	PORTA.PIN3CTRL |= 0b00000011; //SET ISC to 0x3 to have falling edge trigger. ISC is last 3 bits. Set to 011.
}
/************************************************************************/
/* Initializes the RTC peripheral. We are using a 32.768kHz RTC and we set the interrupt to trigger at
every 122us. We are giving higher priority to the RTC interrupt.                                                                     */
/************************************************************************/
void init_RTC(){
	//RTC initialize
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;//32.768 kHz RTC
	while (RTC.STATUS > 0);//wait until status OK
	RTC.PER = 4;  // 122 us per tick
	RTC.INTCTRL |= RTC_OVF_bm;
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc|RTC_RTCEN_bm | RTC_RUNSTDBY_bm;
	CPUINT.LVL1VEC = RTC_CNT_vect_num;//Give priority to RTC Interrupt
}
/************************************************************************/
/* Initializes the lights at the panel. These lights are controlled by a PWM output.                                                                     */
/************************************************************************/
void init_Lights(){
	PORTD.DIR = (1<<LED_ON_OFF);
	PORTD.OUT = 0x00; // Close All LEDs
}
/************************************************************************/
/* This function initializes above functions, enabling interrupts and initializes global state variables.                                                                     */
/************************************************************************/
void Initialize(){
	
	init_CLK();
	init_LED();
	init_Motor();
	
	init_IR();
	init_Lights();
	init_Buzzer();
	SREG |= (1<<GLOBAL_INT_ENABLE); //Enable Interrupts
	 /* Enable Global Interrupts */
	//Initial state
	State.currentState = OFF;
	LightState.currentState = OFF;
	NECState.currentState = IDLE;
	InputState.currentState = INITIAL_STATE;
	BrightModeState.currentState = ONE;
	PreviousBrightModeState.currentState = BrightModeState.currentState;
	//EEPROM_read();
	init_RTC();
}
