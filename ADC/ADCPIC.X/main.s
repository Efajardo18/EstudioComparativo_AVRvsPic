PROCESSOR 16F887
#include <xc.inc>

; CONFIG1
  CONFIG  FOSC = INTRC_NOCLKOUT ; Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
  CONFIG  PWRTE = OFF           ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  MCLRE = OFF           ; RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
  CONFIG  CP = OFF              ; Code Protection bit (Program memory code protection is disabled)
  CONFIG  CPD = OFF             ; Data Code Protection bit (Data memory code protection is disabled)
  CONFIG  BOREN = OFF           ; Brown Out Reset Selection bits (BOR disabled)
  CONFIG  IESO = OFF            ; Internal External Switchover bit (Internal/External Switchover mode is disabled)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
  CONFIG  LVP = OFF             ; Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

; CONFIG2
  CONFIG  BOR4V = BOR40V        ; Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
  CONFIG  WRT = OFF             ; Flash Program Memory Self Write Enable bits (Write protection off)
  
;-------------------------------------------------------------------------------
;Variables
;-------------------------------------------------------------------------------
PSECT udata
  
cont1:
    DS 1
    
cont2:
    DS 1
    
temp_w:
    DS 1

temp_s:
    DS 1

VADC:
    DS 1

;-------------------------------------------------------------------------------
;Vector de Reset
;-------------------------------------------------------------------------------
PSECT code, delta=2, abs ;el delta=2 es obligatorio y abs indica que inicia en 0x0000
ORG 0x0000
    resetVector:
	goto setup
    
PSECT code, class=CODE, delta=2
ORG 0x0004
save:
    MOVWF   temp_w
    SWAPF   STATUS, W
    MOVWF   temp_s
    
interrupt:
    BCF	    PORTC,  7
    BTFSS   PIR1, 6
    GOTO    load
    MOVF    ADRESH, W
    MOVWF   VADC
    BTFSS   ADCON0, 2
    GOTO    can13
    MOVF    VADC,   W
    MOVWF   PORTA
    GOTO    last
    
can13:
    BTFSC   ADCON0, 2
    GOTO    last
    MOVF    VADC,   W
    MOVWF   PORTD
    
last:
    MOVLW   0b00000100
    XORWF   ADCON0, F
    BCF	    PIR1,   6
    BSF	    PORTC,  7
    
load:
    SWAPF   temp_s, W
    MOVWF   STATUS
    SWAPF   temp_w, F
    SWAPF   temp_w, W
    RETFIE
    
PSECT code, class=CODE, delta=2
ORG 0x0020
setup:    
    CALL    TMR_set
    CALL    IO_set
    CALL    INT_set
    CALL    ADC_set
    BANKSEL PORTA
    GOTO    main

main:
    BTFSS   ADCON0, 1
    BSF	    ADCON0, 1
    GOTO    main
    
ADC_set:
    BANKSEL ADCON0	;Banco 0, configuración ADC
    MOVLW   0b10110101    
    MOVWF   ADCON0
    BANKSEL ADCON1	;Banco 1, configuración ADC y entradas
    CLRF    ADCON1
    RETURN

IO_set:
    BANKSEL TRISA	;Banco 1, configuración salidas
    CLRF    TRISA
    MOVLW   0b00100001    
    MOVWF   TRISB
    CLRF    TRISC
    CLRF    TRISD
    BANKSEL ANSEL	;Banco 3, configuraciones de entradas analogicas
    CLRF    ANSEL
    MOVLW   0b00110000     
    MOVWF   ANSELH
    BANKSEL PORTA	;Banco 0, limpieza de salidas
    CLRF    PORTA
    CLRF    PORTC
    CLRF    PORTD
    RETURN
    
INT_set:
    BANKSEL PIE1	;Banco 1, configuración de interrupciones
    MOVLW   0b11000000
    MOVWF   INTCON
    MOVLW   0b01000000
    MOVWF   PIE1
    BANKSEL PIR1	;Banco 0, limpieza de banderas
    CLRF    PIR1
    RETURN
    
TMR_set:
    BANKSEL TRISA	;Banco 1, modificación del oscilador 
    MOVLW   0b01111000
    MOVWF   OSCCON
    RETURN
    
END

