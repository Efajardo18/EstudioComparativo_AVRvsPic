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
int		valorMux = 0;

//**************************************************************************************
//PROTOTIPOS
//**************************************************************************************

void SETUP_ADC();

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
	int valorMux= ADMUX & 1<<MUX0;

	if(valorMux==0) 
	{
	PORTD = valorAdc;
	}        	

	if(valorMux!=0) 
	{
	PORTB = valorAdc;
	}         

	ADMUX ^= 1<<MUX0;            //Cambia entre 1 y 0 el bit MUX0 (canal 0 y 1 de ADC)
	ADCSRA |= 1<<ADSC;           //Inicia la conversion nuevamente
	PORTC ^= 1<<PORTC5;

}


void SETUP_ADC()

{
	ADMUX |= 1<<ADLAR | 1<<REFS0 | 0<<REFS1;		//JUSTIFICA A LA IZQUIERDA EL RESULTADO, ESTABLECE Vcc COMO REFERENCIA
	ADCSRA |= 1<<ADEN;                              //ACTIVA EL MODULO	ADC
	ADCSRA |= 1<<ADPS2 | 0<<ADPS1 | 1<<ADPS0;       //PRESCALER DE 32
	ADCSRA |= 1<<ADIE | 1<<ADSC ;                   //ACTIVA INTERRUPCIONES E INICIA LA CONVERSIÓN
	PORTC |= (1<<PORTC5);
}