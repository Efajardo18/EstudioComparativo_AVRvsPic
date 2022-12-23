/*
 * I2CATM_com.h
 *
 * Created: 27/10/2022 19:17:16
 *  Author: Olifa
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

//#define TW_START		0x08			//Estas definiciones se pueden ahorrar con el header <util\twi.h>
//#define TW_RSTART		0x10			//Se utilizan las definiciones para evitar problemas del header para evitar problemas
//#define TW_MT_SLA_ACK	0x18
//#define TW_MT_SLA_NACK	0x20
//#define TW_MT_DATA_ACK	0x28
//#define TW_MT_DATA_NACK 0x30
//#define TW_M_ERROR		0x38
//#define TW_MR_SLA_ACK	0x40
//#define TW_MR_SLA_NACK	0x48
//#define TW_MR_DATA_ACK	0x50
//#define TW_MR_DATA_NACK 0x58

//#define TW_STATUS		TWSR & 0xF8

#define I2CATM_SLA_W(ADDR)	((ADDR << 1) | TW_WRITE)
#define I2CATM_SLA_R(ADDR)	((ADDR << 1) | TW_READ)

typedef enum{
	_FREQ_100K,
	_FREQ_250K,
	_FREQ_400K	
} I2C_FREQ;

typedef enum{
	_SCL_PSCL_1,
	_SCL_PSCL_4,
	_SCL_PSCL_16,
	_SCL_PSCL_64	
} I2C_PRESCALER;

void I2CATM_Start(void){
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTA);
	while(!(TWCR & (1 << TWINT)));
	if (((TW_STATUS) != TW_START )&& ((TW_STATUS) != TW_REP_START)){
		PORTD |= (1<< PORTD0);
		return;
	}
	return ;
}

void I2CATM_Stop(void){
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);
}

void I2CATM_Write_Add(uint8_t SlaveAddress){
	TWDR = SlaveAddress;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	if ((TW_STATUS != TW_MR_SLA_ACK) && (TW_STATUS != TW_MT_SLA_ACK))
	{
		PORTD |= (1<< PORTD1);
		return;
	}
	return;
}	

void I2CATM_Write_Data(uint8_t Data){
	TWDR = Data;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while (!(TWCR &(1 << TWINT)));
	if(TW_STATUS != TW_MT_DATA_ACK){
		PORTD |= (1<< PORTD2);
		return;		
	}
	return;
}

uint8_t I2CATM_Read_Data(int read_ack){
	if (read_ack == 1){
		TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWEA);
		while(!(TWCR & (1 << TWINT)));
		if (TW_STATUS != TW_MR_SLA_ACK){
			PORTD |= (1<< PORTD7);
			return	128;
		}
	}
	else{
		TWCR = (1 << TWINT)|(1 << TWEN);
		while (!(TWCR &(1 << TWINT)));
		if (TW_STATUS != TW_MR_SLA_NACK){
			PORTD |= (1<< PORTD6);
			return 128;
		}
	}
	PORTD = 0x00;
	return TWDR;
}

void I2CATM_Set(I2C_FREQ I2C_Freq_select, I2C_PRESCALER I2C_Pre_select){
	DDRC |= (1 << PORTC4)|(1 << PORTC5);
	PORTC |= (1 << PORTC4)|(1 << PORTC5);
	DDRC &= ~((1 << PORTC4)|(1 << PORTC5));
	
	switch(I2C_Freq_select)
	{
		case _FREQ_100K:
			switch(I2C_Pre_select)
			{
				case _SCL_PSCL_1:
				TWBR = (uint8_t)(((F_CPU/100000)-16)/(2*1));
				TWSR &= ~((1<< TWPS1)|(1<< TWPS0));
				break;
				
				case _SCL_PSCL_4:
				TWBR = (uint8_t)(((F_CPU/100000)-16)/(2*4));
				TWSR = (TWSR | (1<< TWPS0))& ~(1<< TWPS1);
				break;
				
				case _SCL_PSCL_16:
				TWBR = (uint8_t)(((F_CPU/100000)-16)/(2*16));
				TWSR = (TWSR | (1<< TWPS1))& ~(1<< TWPS0);
				break;
				
				case _SCL_PSCL_64:
				TWBR = (uint8_t)(((F_CPU/100000)-16)/(2*64));
				TWSR |= (1<< TWPS1)|(1<< TWPS0);
				break;
				
				default: break;
			}
		break;
		
		case _FREQ_250K:
		switch(I2C_Pre_select)
		{
			case _SCL_PSCL_1:
			TWBR = (uint8_t)(((F_CPU/250000)-16)/(2*1));
			TWSR &= ~((1<< TWPS1)|(1<< TWPS0));
			break;
			
			case _SCL_PSCL_4:
			TWBR = (uint8_t)(((F_CPU/250000)-16)/(2*4));
			TWSR = (TWSR | (1<< TWPS0))& ~(1<< TWPS1);
			break;
			
			case _SCL_PSCL_16:
			TWBR = (uint8_t)(((F_CPU/250000)-16)/(2*16));
			TWSR = (TWSR | (1<< TWPS1))& ~(1<< TWPS0);
			break;
			
			case _SCL_PSCL_64:
			TWBR = (uint8_t)(((F_CPU/250000)-16)/(2*64));
			TWSR |= (1<< TWPS1)|(1<< TWPS0);
			break;
			
			default: break;
		}
		break;
		
		case _FREQ_400K:
		switch(I2C_Pre_select)
		{
			case _SCL_PSCL_1:
			TWBR = (uint8_t)(((F_CPU/400000)-16)/(2*1));
			TWSR &= ~((1<< TWPS1)|(1<< TWPS0));
			break;
			
			case _SCL_PSCL_4:
			TWBR = (uint8_t)(((F_CPU/400000)-16)/(2*4));
			TWSR = (TWSR | (1<< TWPS0))& ~(1<< TWPS1);
			break;
			
			case _SCL_PSCL_16:
			TWBR = (uint8_t)(((F_CPU/400000)-16)/(2*16));
			TWSR = (TWSR | (1<< TWPS1))& ~(1<< TWPS0);
			break;
			
			case _SCL_PSCL_64:
			TWBR = (uint8_t)(((F_CPU/400000)-16)/(2*64));
			TWSR |= (1<< TWPS1)|(1<< TWPS0);
			break;
			
			default: break;
		}
		break;
		default: break;
	}
}

void I2CATM_MasterRec(uint8_t SlaveAddress, uint8_t* dataP, uint8_t len){
	I2CATM_Start();
	I2CATM_Write_Add(I2CATM_SLA_W(SlaveAddress));
	for (int i=0; i < len-1; ++i){
		dataP[i] = I2CATM_Read_Data(1);
	}
	dataP[len-1] = I2CATM_Read_Data(0);
	I2CATM_Stop();
	return;
}

void I2CATM_MasterTra(uint8_t SlaveAddress, uint8_t* dataP, uint8_t len, int repeat_start){
	I2CATM_Start();
	I2CATM_Write_Add(I2CATM_SLA_W(SlaveAddress));
	for (int i = 0; i < len; ++i){
		I2CATM_Write_Data(dataP[i]);	
	}
	if (repeat_start != 1){
		I2CATM_Stop();
	}
	return;
}

