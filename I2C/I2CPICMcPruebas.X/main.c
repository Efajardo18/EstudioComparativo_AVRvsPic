/*
 * File:   main.c
 * Author: Olifa
 *
 * Created on 2 de marzo de 2023, 11:15 AM
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

#define _XTAL_FREQ 4000000

#include <xc.h>
//#include <stdio.h>
//#include <stdint.h>

void I2C_set(const unsigned long clocks);
void I2C_wait(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(unsigned data);
unsigned short I2C_read(unsigned short ack);

void main(void) {
    TRISA = 0;
    I2C_set(100);
    __delay_ms(200);
    while(1){
        I2C_start();
        I2C_write(0x26);
        I2C_write(0xAA);
        I2C_stop();
    }
    return;
}

void I2C_set(const unsigned long clocks){
    TRISC3 = 1;
    TRISC4 = 1;
    SSPCON  = 0b00001000;
    SSPCON  = 0b00101000;
    SSPCON2 = 0b00000000;
    SSPADD  = 0b00001001;
    //SSPADD  = (_XTAL_FREQ / (4* clocks * 100))-1; 
    SSPSTAT = 0b00000000;
}

void I2C_wait(void){
    while ( (SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100) );
}

void I2C_start(void){
    I2C_wait();
    SEN = 1;
}

void I2C_stop(void){
    I2C_wait();
    PEN = 1;
}

void I2C_write(unsigned data){
    I2C_wait();
    SSPBUF = data;
}

//unsigned short I2C_read(unsigned short ack){
//    unsigned short rData;
//    I2C_wait();
//    RCEN = 1;
//    
//    I2C_wait();
//    rData = SSPBUF;
//    
//    I2C_wait();
//    ACKDT = (ack)?0:1;   
//    ACKEN = 1;
//    
//    return rData;
//}