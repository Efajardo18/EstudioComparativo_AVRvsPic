/* 
 * File:   main.c
 * Author: Olifa
 *
 * Created on 24 de julio de 2022, 02:33 PM
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
#include "ADClib.h"

uint8_t VADC;

void setup(void);

void main(void) {
    setup();
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
    ADCON0  = 0b10110001;
    ADCON1  = 0b00000000;
    PORTA   = 0;
    PORTB   = 0;
    PORTC   = 0b10000000;
    PORTD   = 0;
    return;
}

void __interrupt() ISR (void){
    PORTCbits.RC7 ^= 1;
    if(PIR1bits.ADIF==1){
        VADC=ADRESH;       
        if(ADCON0bits.CHS0 == 1){
            PORTA = VADC;
        }
        if(ADCON0bits.CHS0 != 1){
            PORTD = VADC;
        }
        ADCON0bits.CHS0 ^= 1;
        PORTCbits.RC7 ^= 1;
        PIR1bits.ADIF=0;
        
    }
}