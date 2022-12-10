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

;-------------------------------------------------------------------------------
;Vector de Reset
;-------------------------------------------------------------------------------
PSECT code, delta=2, abs ;el delta=2 es obligatorio y abs indica que inicia en 0x0000
ORG 0x0000
resetVector:
    goto setup
    
PSECT code, delta=2, abs
ORG 0x000A
setup:
    CALL    PWM1_set
    CALL    ADC_set
    CALL    T0_set
    CLRF    PORTA
    GOTO    main

main:
    BSF	    ADCON0, 1
    BTFSC   ADCON0, 1
    GOTO    $-1
    
    MOVF    ADRESH, W
    MOVWF   cont1
    MOVF    cont1,  W
    MOVWF   CCPR1L
    MOVWF   PORTA
    
    goto    main
    
    
PWM1_set:
    BANKSEL TRISC
    CLRF    TRISC
    CLRF    TRISA
    MOVLW   0b01111111
    MOVWF   PR2
    BANKSEL PORTC
    CLRF    PORTC
    MOVLW   0b00111100
    MOVWF   CCP1CON
    MOVLW   0b01111100
    MOVWF   CCPR1L
    MOVLW   0b00000111
    MOVWF   T2CON
    RETURN
    
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
    
T0_set:  
    MOVLW   0b11000011
    MOVWF   OPTION_REG
    MOVLW   0b01111000
    MOVWF   OSCCON
    RETURN


    
END


