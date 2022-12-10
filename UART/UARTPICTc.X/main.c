/*
 * File:   main.c
 * Author: Olifa
 *
 * Created on 20 de agosto de 2022, 04:53 PM
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
#include "UARTcom.h"
#include "PICOSC.h"

#define global  _XTAL_FREQ = 8000000

uint8_t Tdata = 0; 

void ADC_set(void);
void INT_set(void);

void main(void) {
    OSC_set(_8M,0);
    ADC_set();
    INT_set();
    UART_BDset(9600, Async, 0,0);
    UART_TXset();
    ADCON0bits.GO = 1;
    while(1){  
    }
    return;
}

void ADC_set(void){
    OPTION_REG |= (1<< _OPTION_REG_nRBPU_POSITION)
            |(1<< _OPTION_REG_INTEDG_POSITION);
    OPTION_REGbits.PS = 3;
    TRISB   |= (1 << _TRISB_TRISB5_POSITION);
    ANSEL   = 0;
    ANSELH  |= (1 << _ANSELH_ANS13_POSITION);
    ADCON1  = 0;
    ADCON0  = (ADCON0|(1 << _ADCON0_ADCS1_POSITION)) & ~(1 << _ADCON0_ADCS0_POSITION);
    ADCON0bits.CHS = 13;
    ADCON0  |= (1 << _ADCON0_ADON_POSITION);
    return;
}

void INT_set(void){
    INTCON  |= (1 << _INTCON_GIE_POSITION)|(1<< _INTCON_PEIE_POSITION);
    PIE1    |= (1 << _PIE1_ADIE_POSITION);
}

void __interrupt() ISR (void){
    Tdata= ADRESH;
    UART_Tra(Tdata);
    PIR1 &= ~(1<< _PIR1_ADIF_POSITION);
    ADCON0bits.GO = 1;
}
