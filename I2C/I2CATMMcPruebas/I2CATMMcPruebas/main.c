/*
 * main.c
 *
 * Created: 3/6/2023 9:05:29 AM
 *  Author: Olifa
 */ 

#include <xc.h>
#include <avr/io.h>

unsigned tData = 0xAA;

void I2CATM_set(void);
void I2CATM_start(void);
void I2CATM_stop(void);
void I2CATM_writeAddress(unsigned data);
void I2CATM_writeData(unsigned data);

int main(void)
{
    I2CATM_set();
	while(1)
    {
		I2CATM_start();
		I2CATM_start();
		I2CATM_stop();
		I2CATM_writeAddress(0x26);
		I2CATM_writeData(0xAA);
    }
}

void I2CATM_set(void)
{
	TWBR = 0x01;
	TWSR = 0x00;
}

void I2CATM_start(void)
{
	TWCR	= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8)!= 0x08);
}

void I2CATM_writeAddress(unsigned data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8)!= 0x40);
}

void I2CATM_writeData(unsigned data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	while ((TWSR & 0xF8) != 0x28);
}

void I2CATM_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(!(TWCR & (1<<TWSTO)));		
}
