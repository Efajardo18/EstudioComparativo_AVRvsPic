/*
 * File:   main.c
 * Author: Olifa
 *
 * Created on 21 de octubre de 2022, 12:10 PM
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

#define global _XTAL_FREQ = 8000000

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "SPIcom.h"
#include "PICOSC.h"

char tData = 10;
char rData = 15;

void IO_set();

void main(void) {
    OSC_set(_8M,0);
    IO_set();
    SPIPIC_set(1, Mid, A, Slave, _F64);
    SSPBUF = tData;
    while(1)
    {
        if(rData == 15){
            PORTA = 7;
        }
        else{
            PORTA = 0;
            while(1){
                
            }
        }
    }
    return;
}

void IO_set(void){
    TRISA = 0x00;
}

void __interrupt() ISR (void){
    //if(PIR1bits.SSPIF == 1){
        rData = SSPBUF;
        SSPBUF = tData;
        PIR1 &= ~(1<< _PIR1_SSPIF_POSITION);   
    //}
}
