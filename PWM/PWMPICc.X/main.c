/* 
 * File:   main.c
 * Author: Olifa
 *
 * Created on 7 de agosto de 2022, 12:16 AM
 */

//******************************************************************************
//BITS DE CONFIGURACION
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <stdint.h>
#include <stdio.h>

uint8_t VADC;

void setup(void);
void PWM_setup(void);
void GOCHK(void);

void main(void) {
    setup();
    PWM_setup();
    while(1){
        GOCHK();
    }
    return;
}

void setup(void){
    OSCCON  = 0b01111000;
    TRISA   = 0b00000000;   
    TRISB   = 0b00100001;   //RB5 Y RB0 ESTABLECIDOS COMO ENTRADAS
    TRISC   = 0b00000000;
    TRISD   = 0b00000000;
    ANSEL   = 0b00000000;   
    ANSELH  = 0b00110000;   //CANAL ANALÓGICO 12 y 13 ACTIVOS
    INTCON  = 0b11000000;   //GIE Y PEIE ACTIVOS
    PIE1    = 0b01000000;   //ADIE ACTIVO
    ADCON0  = 0b10110101;
    ADCON1  = 0b00000000;
    PORTA   = 0;
    PORTB   = 0;
    PORTC   = 0;
    PORTD   = 0;
    return;
}

void GOCHK(void){
    if(ADCON0bits.GO==0){
            ADCON0bits.GO=1;
        }
}

void PWM_setup(void){
    PR2     = 0b11111001;
    CCP1CON = 0b00111100;
    CCPR1L  = 0b01111100;
    T2CON   = 0b00000111;
}

void __interrupt() ISR (void){
    PORTDbits.RD4 ^= 1;
    if(PIR1bits.ADIF==1){
        VADC=ADRESH;
        PORTA = VADC;
        CCPR1L = VADC;
        PORTDbits.RD4 ^= 1;
        PIR1bits.ADIF=0;
        
    }
}

