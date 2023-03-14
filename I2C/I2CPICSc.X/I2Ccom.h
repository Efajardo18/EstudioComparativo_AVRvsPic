/* 
 * File:   I2Ccom.h
 * Author: Olifa
 *
 * Created on 25 de octubre de 2022, 09:11 PM
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif	

#define Master  0b00001000
#define Slave   0b00000110
#define _10bits 0b00000111
#define _7bits  0b00000110

#include <xc.h>
#include <math.h>

void I2CPIC_Mset(unsigned char IntEnable, unsigned long Clock){
    uint8_t dato;
    TRISC   |= (1<<_TRISC_TRISC4_POSITION)|(1<<_TRISC_TRISC3_POSITION); 
    SSPCON  |= (1 << _SSPCON_SSPEN_POSITION)|(1<< _SSPCON_SSPM3_POSITION);
    SSPCON2 = 0;
    dato = (uint8_t)((8000000/ (4*Clock))-1);
    SSPADD = dato;
    SSPSTAT = 0;
    PIE1bits.SSPIE = IntEnable;
}

void I2CPIC_Sset(int IntEnable, uint8_t SlaveAdd, char addressSize, int StartStopInt){
    SSPSTAT = 0b10000000;
    TRISC3 = 1;
    TRISC4 = 1;
    SSPADD  = 0x26;
    SSPCON  = 0b00110110;
    if(IntEnable == 0){
        PIE1 &= ~(1<<_PIE1_SSPIE_POSITION);
    }
    else if(IntEnable == 1){
        INTCON |= (1<<_INTCON_GIE_POSITION)|(1<<_INTCON_PEIE_POSITION);
        PIE1 |= (1<<_PIE1_SSPIE_POSITION);
        PIR1 |= (1<<_PIR1_SSPIF_POSITION);
    }
}

void I2CPIC_Wait(void){
    while((SSPSTAT & 0x04)||(SSPCON2 & 0x1F));
}

void I2CPIC_Start(void){
    I2CPIC_Wait();
    SSPCON2 |= (1<< _SSPCON2_SEN_POSITION);
}

void I2CPIC_Stop(void){
    I2CPIC_Wait();
    SSPCON2 |= (1<< _SSPCON2_PEN_POSITION);
}

void I2CPIC_Restart(void){
    I2CPIC_Wait();
    SSPCON2 |= (1<< _SSPCON2_RSEN_POSITION);
}

void I2CPIC_MasterWrite(uint8_t Data){
    I2CPIC_Wait();
    SSPBUF = Data;
    while(!PIR1bits.SSPIF)
    {
     PORTA =(1 << _PORTA_RA1_POSITION);   
    }
    PORTA &= ~(1 << _PORTA_RA1_POSITION);
}

unsigned int I2CPIC_MasterRead(uint8_t ACK){
    uint8_t temp;
    I2CPIC_Wait();
    SSPCON2 |= (1<<_SSPCON2_RCEN_POSITION);
    I2CPIC_Wait();
    while(!SSPIF);
    temp = SSPBUF;
    I2CPIC_Wait();
    SSPCON2 |= (ACK << _SSPCON2_ACKDT_POSITION);
    SSPCON2 |= (1 << _SSPCON2_ACKEN_POSITION);
    return temp;
}

uint8_t I2CPIC_SlaveFix(void){
    uint8_t z;
    SSPCON &= ~(1<<_SSPCON_CKP_POSITION);
    if((SSPCON & 0b01000000)||(SSPCON & 0b10000000)){
        z = SSPBUF;
        SSPCON &= ~(1<<_SSPCON_WCOL_POSITION)&~(1<<_SSPCON_SSPOV_POSITION);
        SSPCON |= (1<<_SSPCON_CKP_POSITION);
        return z;
    }
    else{
        return 0;
    }
}

//uint8_t I2CPIC_SlaveListen(){
//    uint8_t z;
//    if(!(SSPSTAT & 0b00100000)&&!(SSPSTAT & 0b00000100)){
//        z = SSPBUF;
//        while(!(SSPSTAT & 0b00000001));
//        z = 
//    }
//}