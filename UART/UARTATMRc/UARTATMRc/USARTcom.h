
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define Async 0
#define Sync  1

#include <math.h>
#include <avr/io.h>

void USART_Szw(int DataSize, int StopBits){
	if (DataSize == 5)
	{
		UCSR0B &= ~(1<<UCSZ02);
		UCSR0C &= ~(1<<UCSZ01) & ~(1<<UCSZ00);
	}
	else if(DataSize == 6)
	{
		UCSR0B &= ~(1<<UCSZ02);
		UCSR0C = (UCSR0C|(1<<UCSZ00)) & ~(1<<UCSZ01);
	}
	else if (DataSize == 7)
	{
		UCSR0B &= ~(1<<UCSZ02);
		UCSR0C = (UCSR0C|(1<<UCSZ01)) & ~(1<<UCSZ00);
	}
	else if (DataSize == 8)
	{
		UCSR0B &= ~(1<<UCSZ02);
		UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	}
	else if (DataSize == 9)
	{
		UCSR0B |= (1<<UCSZ02);
		UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	}
	else
	{
		UCSR0B &= ~(1<<UCSZ02);
		UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	}
	if (StopBits == 1)
	{
		UCSR0C &= ~(1<< USBS0); 
	}
	else if (StopBits == 2)
	{
		UCSR0C |= (1<< USBS0);
	}
	return;
}

//Doble velocidad no es compatible para comunicarse con otros microcontroladores de comunicacion continua, utilizar 
//unicamente para comunicarse con la computadora o micrcontroladores de transmision unica.

void USART_BDset(int Baud, char sync, char DoubleSpeed, int DataSize, int StopBits){
	if ((sync == Async)&&(DoubleSpeed == 0))
	{
		UCSR0C &= ~(1<<UMSEL00) & ~(1<<UMSEL01);
		UCSR0A &= ~(1<< U2X0); 
		uint16_t PerBaud= lrint((F_CPU/(16L* Baud))-1); //La funcion lrint aproxima al numero entero mas cercano
		UBRR0L = (uint8_t)(PerBaud);
		UBRR0H = (uint8_t)(PerBaud>>8);
		
	}
	else if ((sync == Async)&&(DoubleSpeed == 1))
	{
		UCSR0C &= ~(1<<UMSEL00) & ~(1<<UMSEL01);
		UCSR0A |= (1<< U2X0);
		uint16_t PerBaud= lrint((F_CPU/(8L* Baud))-1);
		UBRR0L = (uint8_t)(PerBaud);
		UBRR0H = (uint8_t)(PerBaud>>8);
	}
	else if (sync == Sync)
	{
		UCSR0C = (UCSR0C|(1<<UMSEL00)) & ~(1<<UMSEL01);
		uint16_t PerBaud= lrint((F_CPU/(2L* Baud))-1);	
		UBRR0L = (uint8_t)(PerBaud);
		UBRR0H = (uint8_t)(PerBaud>>8);
	}
	USART_Szw(DataSize, StopBits);
	UCSR0C &= ~(1<<UPM01) & ~(1<<UPM00) & ~(1<<UCPOL0);
	return;	
}

void USART_Rset(char RecEn,  char RecIntEn){
	if ((RecEn == 1 ) && (RecIntEn == 1))
	{
		UCSR0B |= (1<<RXCIE0)|(1<<RXEN0);
		sei();		
	}
	else if((RecEn == 1 ) && (RecIntEn == 0))
	{
		UCSR0B = (UCSR0B|(1<<RXEN0)) & ~(1<<RXCIE0);
	}
	else if (RecEn == 0)
	{
		UCSR0B &= ~(1<<RXEN0) & ~(1<<RXCIE0);
	}
	return;
}

void USART_Tset(char TraEn, char TraIntEn){
	if ((TraEn == 1 ) && (TraIntEn == 1))
	{
		UCSR0B |= (1<<TXCIE0)|(1<<TXEN0);
		sei();
	}
	else if((TraEn == 1 ) && (TraIntEn == 0))
	{
		UCSR0B = (UCSR0B|(1<<TXEN0)) & ~(1<<TXCIE0);
	}
	else if (TraEn == 0)
	{
		UCSR0B &= ~(1<<TXEN0)&~(1<<TXCIE0);
	}
	return;
}

uint8_t USART_RecInt(void){
	return UDR0;
}

uint8_t USART_Rec(void){
	while(!RXC0);
	return UDR0;
}
