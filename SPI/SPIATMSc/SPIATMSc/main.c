/*
 * main.c
 *
 * Created: 9/1/2022 7:29:06 PM
 *  Author: Olifa
 */ 

//**************************************************************************************
//DEFINICIONES
//************************************************************************************** 
#define F_CPU 8000000UL

//**************************************************************************************
//INCLUDES
//**************************************************************************************
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "SPIATMcom.h"

//**************************************************************************************
//VARIABLES
//**************************************************************************************

uint8_t rData = 15;
uint8_t tData = 10;

//**************************************************************************************
//PROTOTIPOS
//**************************************************************************************

void IO_set(void);
void SETUP_ADC(void);

//**************************************************************************************
//PROGRAMA
//**************************************************************************************

int main(void)
{
	IO_set();
    SPIATM_set(1,MostFirst,A,Slave,_F64);
	_delay_ms(10);
	sei();
	//SETUP_ADC();
	SPDR = tData;
	while(1)
    {
		PORTD = rData;
		
		
		/*if (rData == 15)
		{
			PORTD = 7;
		}
		else
		{
			PORTD = 0;
			//while(1){
				
			//}
		}*/
    }
}

void IO_set(void)
{
	DDRC = 0x00;
	PORTC = 0x00; 
	DDRD = 0xFF;
}

void SETUP_ADC(void)
{
	ADMUX |= 1<<ADLAR | 1<<REFS0 | 0<<REFS1;		//JUSTIFICA A LA IZQUIERDA EL RESULTADO, ESTABLECE Vcc COMO REFERENCIA
	ADCSRA |= 1<<ADEN;                              //ACTIVA EL MODULO	ADC
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;       //PRESCALER DE 128
	ADCSRA &= ~(1<<ADIE);							//DESACTIVA INTERRUPCIONES
	ADCSRA |= 1<<ADSC ;								//INICIA LA CONVERSIÓN
}

//**************************************************************************************
//INTERRUPCIONES
//**************************************************************************************

ISR(SPI_STC_vect)
{
	rData = SPDR;
	SPDR = tData;
}