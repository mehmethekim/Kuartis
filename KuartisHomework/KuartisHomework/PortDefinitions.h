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

//IR INPUT

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

//CLKSEL for prescalar calculations
#define DIV8 0x3 // CLKSEL0 = 1 CLKSEL_1 = 1 CLKSEL_2 = 0

#define TCA0_ENABLE 0
#define CLKSEL_0 1
#define CLKSEL_1 2
#define CLKSEL_2 3

//Timer interrupt enable CMP0
#define CMP_0 4
