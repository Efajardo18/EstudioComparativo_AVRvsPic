;
; PWMATM.asm
;
; Created: 2/08/2022 21:16:11
; Author : Olifa
;

.org 0
; Replace with your application code
start:
	CALL	IO_SET
	CALL	PWM_SET
	CALL	ADC_SET
	CALL	VAR_SET

Loop:
	CALL	POLL
	LDS		R26,	ADCH
	OUT		PORTB,	R26
	OUT		OCR0A,	R26
	RJMP	Loop

IO_SET:
	LDI		R16,	0x20 // el PC5 es salida, el resto son entradas
	OUT		DDRC,	R16
	LDI		R16,	0x00 // Colocamos resistencias internas para los adc y borramos el contenido del pin 5
	OUT		PORTC,	R16
	LDI		R16,	0xFF
	OUT		DDRB,	R16
	OUT		DDRD,	R16
	RET

PWM_SET:
	LDI		R16,	0b10000011
	OUT		TCCR0A,	R16
	LDI		R16,	0b00000011
	OUT		TCCR0B,	R16
	LDI		R16,	0x02
	OUT		OCR0A,	R16
	LDI		R16,	0x00
	OUT		PORTD,	R16
	OUT		PORTB,	R16
	RET

ADC_SET:
	LDI		R17,	0b01100000
	STS		ADMUX,	R17
	LDI		R16,	0b10000001
	STS		ADCSRA,	R16
	LDI		R18,	0b01100001
	RET

VAR_SET:
	LDI		R16,	0b11000101
	LDI		R19,	0x00
	LDI		R20,	0b00001111
	LDI		R21,	0x00
	LDI		R22,	0x00
	LDI		R24,	0x00
	LDI		R25,	0x01
	LDI		R27,	0b00000000
	LDI		R28,	0b00100000
	RET

POLL:
	STS		ADCSRA,	R16
	LDS		R23,	ADCSRA
	SBRS	R23,	4
	RJMP	PC-3
	RET

TIME:
	NOP
	INC		R19
	CP		R19,	R20
	BREQ	PC+2
	RJMP	PC-4
	NOP
	RET