/*
 * main.c
 *
 * Created: 10/25/2022 4:40:19 PM
 *  Author: Olifa
 */ 
#define F_CPU 8000000UL
#define _Slave_Address 0x26

#include <xc.h>
#include <avr/io.h>
#include "I2CATM_com.h"

uint8_t rData = 0;
uint8_t tData = 170;

//typedef struct{
	//uint8_t dato1,
	//uint8_t dato2	
//} recepcion;

void setup (void);

int main(void)
{
	I2CATM_Set(_FREQ_250K, _SCL_PSCL_1);
    while(1)
    {
         I2CATM_Start();
		 I2CATM_Write_Add(_Slave_Address);
		 I2CATM_Write_Data(170);
		 I2CATM_Stop();
    }
}

void setup(void)
{
	DDRD = 0xFF; //Salidas para banderas debuggeo de I2C.
	DDRB = 0xFF; //Salida para mostar el dato recibido por medio de la comunicación I2C.
}