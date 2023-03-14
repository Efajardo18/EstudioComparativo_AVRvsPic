#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define LeastFirst 1
#define MostFirst 0
#define Slave 0
#define Master 1
#define _F2   00000000
#define _F4   00000000
#define _F8   00000001
#define _F16  00000001
#define _F32  00000010
#define _F64  00000010
#define _F128 00000011
#define C	  0
#define D	  1
#define B	  2
#define A	  3

#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void SPIATM_sck(char FrqSCK){
	if ( FrqSCK == _F2 || FrqSCK == _F8 || FrqSCK == _F32)
	{
		SPSR |= (1<<SPI2X);
	}
	else{
		SPSR &= ~(1<<SPI2X);
	}
	SPCR |= FrqSCK;
}

void SPIUATM_set(int Baud, int DataMode, char FrameSelect){
	cli();
	uint16_t PerBaud = lrint((F_CPU/(2L*Baud)) -1);
	UBRR0L = 0;
	UBRR0H = 0;
	DDRD |= (1<< PORTD4);
	UCSR0C = (UCSR0C|(1<<UMSEL01))|(1<<UMSEL00);	
	if (DataMode == 0)
	{
		UCSR0C &= ~(1<<UCPHA0) & ~(1<<UCPOL0);	
	}
	else if (DataMode == 1)
	{
		UCSR0C = (UCSR0C & ~(1<<UCPHA0)) | (1<< UCPOL0);
	}
	else if (DataMode == 2)
	{
		UCSR0C = (UCSR0C | (1<<UCPHA0))	& ~(1<< UCPOL0);
	}
	else if (DataMode == 3)
	{
		UCSR0C |= (1<< UCPHA0) | (1<< UCPOL0);
	}
	
	if (FrameSelect == LeastFirst)
	{
		UCSR0C |= (1<< UDORD0);
	}
	else if (FrameSelect == MostFirst)
	{
		UCSR0C &= ~(1<< UDORD0);
	}
	
	UCSR0B |= (1<<RXCIE0)|(1<< TXCIE0);
	UBRR0L = (uint8_t) PerBaud;
	UBRR0H = (uint8_t)(8>> PerBaud);
	sei();
}

uint8_t SPIUATM_tnr(uint8_t TraData){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = TraData;
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

void SPIATM_set(char IntEnable, char FrameSelect, int DataMode, char Role, char FrqSCK){
	if (Role == Master)
	{
		DDRB &= ~(1<<DDB4);							// DB4 MISO (ENTRADA)
		DDRB |= (1<<DDB5)|(1<<DDB3)|(1<<DDB2);		// DB5 SCK (SALIDA), DB3 MOSI(SALIDA), DB2 (SALIDA)
		SPCR |= (1<<MSTR);
		SPIATM_sck(FrqSCK);
	}
	else if(Role == Slave)
	{
		DDRB |= (1<<DDB4);
		DDRB &= ~(1<<DDB5) & ~(1<<DDB3) & ~(1<<DDB2);
		SPCR &= ~(1<<MSTR);
	}
	if ((IntEnable == 1) && (FrameSelect == LeastFirst))
	{
		SPCR |= (1<<SPIE)|(1<<SPE)|(1<<DORD);
	}
	else if ((IntEnable == 0) && (FrameSelect == MostFirst))
	{
		SPCR = ((SPCR & ~(1<<SPIE))|(1<< SPE)) & ~(1<<DORD) ;
	}
	else if ((IntEnable == 1) && (FrameSelect == MostFirst))
	{
		SPCR = ((SPCR| (1<<SPIE))| (1<< SPE)) & ~(1<<DORD);
	}
	else if ((IntEnable == 0) && (FrameSelect == LeastFirst))
	{
		SPCR = (SPCR & ~(1<< SPIE))| (1<< SPE) | (1<< DORD);		
	}
	
	if (DataMode == 0)
	{
		SPCR &= ~(1<<CPHA) & ~(1<<CPOL);
	}
	else if (DataMode == 1)
	{
		SPCR = (SPCR & ~(1<<CPHA)) | (1<< CPOL);
	}
	else if (DataMode == 2)
	{
		SPCR = (SPCR | (1<<CPHA))	& ~(1<< CPOL);
	}
	else if (DataMode == 3)
	{
		SPCR |= (1<< CPHA) | (1<< CPOL);
	}
}

uint8_t SPIATM_tnr(uint8_t TraData){
	SPDR = TraData;
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}

void SPIATM_SSstart(char Port ,int SlaveNum){
	if (Port == C)
	{
		PORTC &= ~(1<<SlaveNum);	
	}
	else if (Port == B)
	{
		PORTB &= (1<<SlaveNum);
	}
	else if (Port == D)
	{
		PORTD &= ~(1<<SlaveNum);
	}
}

void SPIATM_SSstop(char Port ,int SlaveNum){
	if (Port == C)
	{
		PORTC |= (1<<SlaveNum);
	}
	else if (Port == B)
	{
		PORTB |= (1<<SlaveNum);
	}
	else if (Port == D)
	{
		PORTD |= (1<<SlaveNum);
	}
}

int SPIATM_check(char Port, int SlaveNum){			
	if (Port == C)
	{
		if (!(PORTC & (1<<SlaveNum))){			//Chequea si hay una transmision en progreso
			return 1;
		}
		else{
			return 0;
		}
	}
	else if (Port == B)
	{
		if (!(PORTB & (1<<SlaveNum))){			//Chequea si hay una transmision en progreso
			return 1;
		}
		else{
			return 0;
		}
	}
	else if (Port == D)
	{
		if (!(PORTD & (1<<SlaveNum))){			//Chequea si hay una transmision en progreso
			return 1;
		}
		else{
			return 0;
		}
	}
	else if (Port == D)
	{
		if (!(PORTD & (1<<SlaveNum))){			//Chequea si hay una transmision en progreso
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}

uint8_t SPIATM_Rx(){
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

void SPIATM_Tx(uint8_t tData){
	SPDR = tData;
	while (!(SPSR & (1<<SPIF)));
}


