#ifndef F_CPU
#define F_CPU 20000000UL // 20MHz Clock
#endif
//Motor Relay PORT Definitions

#define MOTOR_RELAY_1 6 //PORTD6
#define MOTOR_RELAY_2 5 //PORTD5
#define MOTOR_RELAY_3 4 //PORTD4
#define MOTOR_RELAY_4 2 //PORTD2

//LED PORT Definitions

#define LED_1 0 //PORTA0
#define LED_2 2 //PORTA2
#define LED_3 1 //PORTA1
#define LED_4 5 //PORTA5

//IR INPUT PORT

#define IR_INPUT 3 //PORTA3

// IR Codes

#define IR_POWER 129UL
#define IR_PLUS	 133UL
#define IR_MINUS 138UL
#define IR_LIGHT 153UL

//USART PINS

#define USART_TRAN 0 //PORTC0
#define USART_REC  1 //PORTC1

//Special Light Control Pins

#define LED_ON_OFF		1 //PORTD1
#define LED_BRIGHTNESS  4 //PORTF4

#define GLOBAL_INT_ENABLE 7 //SREG bit7

//Buzzer
#define BUZZER 7 //PORTD7

#define TIM0_PRESCALER  1024
#define TIM0_MS         1
#define TIM0_MS_FACTOR  ( (F_CPU) / (TIM0_PRESCALER) )
#define TIM0_COMP_VAL   19534//(TIM0_MS * TIM0_MS_FACTOR)

#define ADDRESS_bm  0x81660000 //129-102
#define IR_POWER_bm 0x0000817E //129-inv 129
#define IR_PLUS_bm	0x0000A15E //133-inv 133
#define IR_MINUS_bm 0x000051AE //138-inv138
#define IR_LIGHT_bm 0x00009966 //153-inv 153

//Time Definitions
#define BOOST_MAX_15MIN 900000000UL
#define BOOST_MAX_30SEC	30000000UL
#define ONE_SEC 1000000UL
