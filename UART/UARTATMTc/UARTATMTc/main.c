/*
 * main.c
 *
 * Created: 8/21/2022 8:09:34 PM
 *  Author: Olifa
 */ 

//**************************************************************************************
//DEFINICIONES
//**************************************************************************************
#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"USARTcom.h"
 
//**************************************************************************************
//VARIABLES
//**************************************************************************************

uint8_t valorAdc = 0;

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
	SETUP_ADC();
	USART_BDset(9600,Async,0,8,1);
	USART_Rset(0,0);
	USART_Tset(1,0);
    while(1)
    {
    }
}


void IO_set(void)
{
	DDRC = 0;
	PORTC = 0;
	sei();	
	return;
}

void SETUP_ADC(void)

{
	ADMUX |= 1<<ADLAR | 1<<REFS0 | 0<<REFS1;		//JUSTIFICA A LA IZQUIERDA EL RESULTADO, ESTABLECE Vcc COMO REFERENCIA
	ADCSRA |= 1<<ADEN;                              //ACTIVA EL MODULO	ADC
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;       //PRESCALER DE 128
	ADCSRA |= 1<<ADIE | 1<<ADSC ;                   //ACTIVA INTERRUPCIONES E INICIA LA CONVERSIÓN
	return;
}

//**************************************************************************************
//INTERRUPCIONES
//**************************************************************************************

ISR(ADC_vect)
{	
	valorAdc = ADCH;					//Guardamos el resultado
	USART_Tra(valorAdc);
	ADCSRA |= 1<<ADSC;
}