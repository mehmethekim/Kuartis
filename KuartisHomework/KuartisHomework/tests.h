#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#include <util/delay.h>
void Test_One(){
	
	//Press power button
	InputState.currentState = POWER;
	WriteInput();
	setState(); // 1
	_delay_ms(10);
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); // 2
	_delay_ms(10);
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); // 3
	_delay_ms(10);
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //2
	_delay_ms(1);
	//Press Light Button
	InputState.currentState = LIGHT;
	WriteInput();
	setState();
	_delay_ms(1000);
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); //3
	_delay_ms(200);
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); //4
	
	//Press + button
	InputState.currentState = INCREMENT;
	WriteInput();
	setState(); //4
	_delay_ms(1000);
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //3
	_delay_ms(1000);
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //2
	_delay_ms(1000);
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //1
	_delay_ms(1000);
	//Press - button
	InputState.currentState = DECREMENT;
	WriteInput();
	setState(); //1
	_delay_ms(1000);
	//Press power button
	InputState.currentState = POWER;
	WriteInput();
	setState(); // OFF
	_delay_ms(1000);
}