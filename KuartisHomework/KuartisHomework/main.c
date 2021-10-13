/*
 * KuartisHomework.c
 *
 * Created: 05/10/2021 15:59:25
 * Author : Mehmet Hekimoðlu
 */ 

#include <avr/io.h>
#include "PortDefinitions.h"
#include "Functions.h"
#include "init.h"
#include "interrupts.h"

//#include "tests.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#ifndef F_CPU
#define F_CPU 20000000UL // 20MHz Clock
#endif

//+/- light adjustment
//Hold that value inside of the chip, maybe write it on EEPROM or ROM


int main(void)
{
	Initialize();
    while (1) 
    {
	
    }
}

