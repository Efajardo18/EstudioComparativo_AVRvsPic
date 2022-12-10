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
  
lec1:
    DS 1
    
lec2:
    DS 1
    
flags:
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
    BTFSS   PIR1,   5
    GOTO    load
    MOVF    RCREG,  W
    MOVWF   lec1
    GOTO    load
    
load:
    SWAPF   temp_s, W
    MOVWF   STATUS
    SWAPF   temp_w, F
    SWAPF   temp_w, W
    RETFIE
    
PSECT mainProg, class=CODE, delta=2
ORG 0x0019
setup:
    CALL    OSC_set
    CALL    UART_Rset
    CALL    INT_set
    BANKSEL PORTA
    CLRF    flags
    CLRF    lec1
    GOTO    main

main:
    MOVF    lec1,   W
    MOVWF   PORTA
    GOTO    main
    
UART_Rset:
    BANKSEL BAUDCTL
    CLRF    BAUDCTL
    BANKSEL SPBRG
    MOVLW   0x00
    MOVWF   SPBRGH
    MOVLW   0b00001100	    
    MOVWF   SPBRG	    ;12 en SPBRG para 9600
    CLRF    TXSTA
    BANKSEL RCSTA
    BSF	    RCSTA,  7	    ;Encendemos el bit SPEN (Serial enable)
    BSF	    RCSTA,  4	    ;Bit de envio continuo
    BCF	    RCSTA,  5	    ;Bit de envio único
    RETURN
    
INT_set:
    BANKSEL INTCON
    MOVLW   0b11000000
    MOVWF   INTCON
    BANKSEL PIE1
    BSF	    PIE1,   5
    RETURN 
    
OSC_set:
    BANKSEL OSCCON
    MOVLW   0b01110000
    IORWF   OSCCON, 1
    CLRF    TRISA
    RETURN

END


