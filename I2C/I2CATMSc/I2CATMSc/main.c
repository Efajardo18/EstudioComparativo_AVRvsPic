/*
 * main.c
 *
 * Created: 12/11/2022 2:42:25 PM
 *  Author: Olifa
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <xc.h>
#include "I2CATM_comS.h"

void I2CATM_SlaveSet(uint8_t address);
void I2C_SlaveStop(void);
ISR(TWI_vect);

int main(void)
{
	I2CATM_SlaveSet(0x32);
	sei();
		
    while(1)
    {
        txbuffer[0]=128; 
    }
}