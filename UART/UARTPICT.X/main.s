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
PSECT udata_shr
  
cont1:
    DS 1
    
cont2:
    DS 1
    
temp_w:
    DS 1

temp_s:
    DS 1

;-------------------------------------------------------------------------------
;Vector de Reset
;-------------------------------------------------------------------------------
PSECT code, delta=2, abs ;el delta=2 es obligatorio y abs indica que inicia en 0x0000
ORG 0x0000
resetVector:
    goto setup
    
PSECT intCodeLo, class=CODE, delta=2
ORG 0x0004
save:
    MOVWF   temp_w
    SWAPF   STATUS, W
    MOVWF   temp_s
    
interrupt:
    BTFSS   PIR1, 6
    GOTO    load
    MOVF    ADRESH, W
    MOVWF   cont1
    BANKSEL TXSTA
    BTFSS   TXSTA,  1
    GOTO    load
    BANKSEL TXREG
    MOVF    cont1,  W
    MOVWF   TXREG
    BCF	    PIR1, 6
    BSF	    ADCON0, 1
    GOTO    load
    
load:
    SWAPF   temp_s, W
    MOVWF   STATUS
    SWAPF   temp_w, F
    SWAPF   temp_w, W
    RETFIE
    
PSECT mainProg, class=CODE, delta=2
setup:    
    CALL    ADC_set
    CALL    IO_set
    CALL    INT_set
    CALL    TMR_set
    CALL    UART_Tset
    BSF	    ADCON0, 1
    GOTO    main

main:
    GOTO    main
    
ADC_set:
    BANKSEL ANSEL	;Banco 4, configuraciones de entradas analogicas
    CLRF    ANSEL
    MOVLW   0b00110000     
    MOVWF   ANSELH
    BANKSEL ADCON1
    CLRF    ADCON1
    MOVLW   0b00100001    
    MOVWF   TRISB
    BANKSEL ADCON0
    MOVLW   0b10110101    
    MOVWF   ADCON0
    RETURN

IO_set:
    BANKSEL TRISA
    CLRF    TRISA
    CLRF    TRISC
    CLRF    TRISD
    BANKSEL PORTA
    CLRF    PORTA
    CLRF    PORTC
    CLRF    PORTD
    RETURN
    
INT_set:
    BANKSEL INTCON
    MOVLW   0b11000000
    MOVWF   INTCON
    BANKSEL PIE1
    MOVLW   0b01000000
    MOVWF   PIE1
    BANKSEL PIR1
    MOVLW   0x00
    MOVWF   PIR1
    RETURN
    
TMR_set:
    BANKSEL OSCCON
    MOVLW   0b11000011
    MOVWF   OPTION_REG
    MOVLW   0b01110000
    IORWF   OSCCON
    RETURN

UART_Tset:
    BANKSEL SPBRG
    MOVLW   0x00
    MOVWF   SPBRGH
    MOVLW   0b00001100	    
    MOVWF   SPBRG	    ;12 en SPBRG para 9600
    MOVLW   0b00000010
    MOVWF   TXSTA
    BANKSEL BAUDCTL
    MOVLW   0b01000000
    MOVWF   BAUDCTL
    BANKSEL RCSTA
    MOVLW   0b10000000
    MOVWF   RCSTA
    BANKSEL TXSTA
    BSF	    TXSTA,  5
    BANKSEL PORTA
    RETURN 
    
END


