/*
 * main.c
 *
 * Created: 8/5/2022 10:16:21 PM
 *  Author: Olifa
 */ 
//**************************************************************************************
//DEFINICIONES
//**************************************************************************************
#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

//**************************************************************************************
//VARIABLES
//**************************************************************************************

uint8_t valorAdc = 0;

//**************************************************************************************
//PROTOTIPOS
//**************************************************************************************

void SETUP_ADC();
void SETUP_PWM();

//**************************************************************************************
//PROGRAMA
//**************************************************************************************
int main()
{
	DDRD = 0xFF;	//Establece los puertos D Y B como salidas
	DDRB = 0xFF;
	DDRC = 0x20;	//Establece el 5 como flag de conversión
	sei();			//Activa las interrupciones 	
	SETUP_ADC();	
	SETUP_PWM();
	while(1)
	{
	}				//Bucle
}

//**************************************************************************************
//INTERRUPCIONES
//**************************************************************************************

ISR(ADC_vect)
{
	PORTC ^= 1<<PORTC5;
	valorAdc = ADCH;					//Guardamos el resultado

	PORTB = valorAdc;
	OCR0A = valorAdc;					//Cambiamos el duty cicle
	
	ADCSRA |= 1<<ADSC;					//Inicia la conversion nuevamente
	PORTC ^= 1<<PORTC5;

}


void SETUP_ADC()

{
	ADMUX |= 1<<ADLAR | 1<<REFS0 | 0<<REFS1;		//JUSTIFICA A LA IZQUIERDA EL RESULTADO, ESTABLECE Vcc COMO REFERENCIA
	ADCSRA |= 1<<ADEN;                              //ACTIVA EL MODULO	ADC
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;       //PRESCALER DE 128
	ADCSRA |= 1<<ADIE | 1<<ADSC ;                   //ACTIVA INTERRUPCIONES E INICIA LA CONVERSIÓN
	PORTC |= 0<<PORTC5;
}

void SETUP_PWM()

{
	TCCR0A	|=	1<<COM0A1 | 0<<COM0A0 | 1<<WGM01 | 1<<WGM00;	//Establecemos Active-HIGH comp. OCR0A y timer; Modo Fast PWM(255)
	TCCR0B	|=	0<<CS02 | 1<<CS01 | 1<<CS00;					//Prescaler de 64 
	OCR0A	=	0x00;											//Duty cicle de 0
}