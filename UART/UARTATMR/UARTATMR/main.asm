;
; UARTATMR.asm
;
; Created: 14/08/2022 13:05:30
; Author : Olifa
;

.org 0
RJMP	setup

.org 0x24	;Interrupcion RX UART
RJMP	UART_rx

setup:
    CALL	IO_set
	CALL	UART_set
	RJMP	loop

loop:
	OUT		PORTB,	R18
	RJMP	loop


UART_set:
	LDI		R16,	0b00000000		;Baudrate de 9615 
	LDI		R17,	0b00110011		;Se coloca 51 en los registros UBRR0H:UBRR0L
	STS		UBRR0H,	R16				
	STS		UBRR0L,	R17			 
	LDI		R16,	0b10010000		;Recepcion activa [4], transmision apagada [3], interrupciones [RXC,TXC,UDRIE] [7:5]
	LDI		R17,	0b00000110		;Asincrono [7:6], sin paridad [5:4], 1 stop [3], 8 bits [2:1], x [0]
	STS		UCSR0B,	R16
	STS		UCSR0C,	R17
	RET

UART_rx:
	LDS		R16,	UCSR0A
	SBRS	R16,	7
	RJMP	UART_rx
	LDS		R18,	UDR0
	RETI	

IO_set:
	SER		R16
	OUT		DDRB,	R16
	SEI
	RET
