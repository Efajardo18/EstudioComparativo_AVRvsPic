/*
 * IncFile1.h
 *
 * Created: 11/12/2022 14:46:23
 *  Author: Olifa
 */ 


#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

uint8_t Buffer_Address;
uint8_t rxbuffer[0xFF];
uint8_t txbuffer[0xFF];

void I2CATM_SlaveSet(uint8_t address){
	TWAR = address;
	TWCR = (1<< TWIE)|(1<< TWEA)|(1<< TWINT)|(1<< TWEN);	
}

void I2C_SlaveStop(void){
	TWCR &= ~((1<< TWEA)|(1<< TWEN));
}

ISR(TWI_vect){
	uint8_t data;
	if(TW_STATUS == TW_SR_SLA_ACK){											//Se recibio correctamente la direccion y el ACK
		Buffer_Address = 0xFF;												//Se prepara para recibir el siguiente byte
		TWCR |= (1<< TWIE)|(1<< TWINT)|(1<< TWEA)|(1<< TWEN); 
	}
	else if(TW_STATUS == TW_SR_DATA_ACK){									//Se recibe correctamente el dato y el ACK (modo receptor)
		data = TWDR;														//Guardar el dato en data
		if(Buffer_Address == 0xFF){											//revisar si ya se recibio una direccion
			Buffer_Address = data;											
			TWCR |= (1<< TWIE)|(1<< TWINT)|(1<< TWEA)|(1<< TWEN);
		}
		else{
			rxbuffer[Buffer_Address] = data;								//Si ya se recibio un dato, se guarda 
			Buffer_Address++;												//Y aumenta la direccion
			if (Buffer_Address < 0xFF){
				TWCR |= (1<< TWIE)|(1<< TWINT)|(1<< TWEA)|(1<< TWEN);	
			}
			else{
				TWCR |= (1<< TWIE)|(1<< TWINT)|(1<< TWEN);					//recibir un ultimo byte de datos
			}
			
		}		
	}
	else if (TW_STATUS == TW_ST_DATA_ACK){									//Si se inidica modo transmisor
		data = TWDR;														
		if (Buffer_Address == 0xFF){										//Si ya se recibio direccion guarda el dato
			Buffer_Address = data;
		}
		TWDR = txbuffer[Buffer_Address];									//Se envia el primer dato del buffer 
		Buffer_Address++;													//se aumenta la direccion 
		if (Buffer_Address < 0xFF){											//Si hay otro dato por enviar lo envia
			TWCR |= (1<< TWIE)|(1<< TWINT)|(1<< TWEA)|(1<< TWEN);
		}
		else{																//si no, manda el ultimo y manda un NACK
			TWCR |= (1<< TWIE)|(1<< TWINT)|(1<< TWEN);
		}
	}
	else{																	//Si hay error, se prepara para recibir otra direccion
		TWCR |= (1<< TWIE)|(1<< TWEA)|(1<< TWEN);
	}
}

