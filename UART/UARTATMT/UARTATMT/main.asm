;
; UARTATMT.asm
;
; Created: 14/08/2022 23:57:24
; Author : Olifa
;

.org 0
; Replace with your application code
setup:
	CALL	UART_set

loop:
	CALL	Data
	CALL	UART_tx
	RJMP	loop

UART_set:
	LDI		R16,	0b00000000		;Baudrate de 9615 
	LDI		R17,	0b00110011		;Se coloca 51 en los registros UBRR0H:UBRR0L
	STS		UBRR0H,	R16				
	STS		UBRR0L,	R17			 
	LDI		R16,	0b00001000		;Recepcion apagada [4], transmision activa [3], interrupciones apagadas [7:5]
	LDI		R17,	0b00000110		;Asincrono [7:6], sin paridad [5:4], 1 stop [3], 8 bits [2:1], x [0]
	STS		UCSR0B,	R16
	STS		UCSR0C,	R17
	RET

UART_tx:
	LDS		R16,	UCSR0A
	SBRS	R16,	5
	RJMP	UART_tx
	STS		UDR0,	R18
	RET

Data:
	LDI		R18,	0x49
	RET

