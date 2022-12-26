/*
 * File:   main.c
 * Author: Olifa
 *
 * Created on 30 de octubre de 2022, 08:09 PM
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

#define _XTAL_FREQ 8000000
#define SlaveAddress 0x19

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "PICOSC.h"
#include "I2Ccom.h"

char tData = 50;
char rData = 0;
short z;

void main(void) {
    OSC_set(_8M,0);
    TRISA = 0;
    I2CPIC_Sset(1, SlaveAddress, _7bits, 0);
    while(1)
    {
        
    }
    return;
}

void __interrupt() I2C_Slave_Read(void){
    if(PIR1 & 0b00001000){
        SSPCON &= ~(1<<_SSPCON_CKP_POSITION);           
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;             
            SSPCON &= ~(1<<_SSPCON_WCOL_POSITION)&~(1<<_SSPCON_SSPOV_POSITION);
            SSPCON |= (1<<_SSPCON_CKP_POSITION);;
        }
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){
            z = SSPBUF;
            while(!BF);
            PORTA = SSPBUF;
            SSPCONbits.CKP = 1;
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = tData;
            SSPCONbits.CKP = 1;
            while(SSPSTATbits.BF);
        }
        SSPIF = 0;
    }
}