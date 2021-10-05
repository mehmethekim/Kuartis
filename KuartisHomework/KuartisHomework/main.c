/*
 * KuartisHomework.c
 *
 * Created: 05/10/2021 15:59:25
 * Author : Mehmet Hekimoðlu
 */ 

#include <avr/io.h>
#include "PortDefinitions.h"
#include "Functions.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>


//Global Variables
int state = 0;
int main(void)
{
	Initialize();
	Test_One();
    while (1) 
    {
		
		
		
    }
}

