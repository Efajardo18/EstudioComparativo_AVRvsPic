/* 
 * File:   SPIcom.h
 * Author: Olifa
 *
 * Created on 23 de agosto de 2022, 04:47 PM
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif	

#define	XC_HEADER_TEMPLATE_H
#define Master  1
#define Slave   0
#define End     1
#define Mid     0
#define _F4     0b00000000
#define _F16    0b00000001
#define _F64    0b00000010
#define A       0
#define B       1
#define C       2
#define D       3
#define E       4

#include <xc.h> 
#include <math.h>

void SPIPIC_set(char IntEnable, char EdgeSampled, int DataMode, char Role, char FrqSCK){
    TRISC5 = 0;         //C5 es salida, SDO
    if(Role == Master){
        TRISC3 = 0;     //C3 es salida, SCK
        if(EdgeSampled == End){
            SSPSTAT |= (1<< _SSPSTAT_SMP_POSITION);
        }
        else if(EdgeSampled == Mid){
            SSPSTAT &= ~(1<< _SSPSTAT_SMP_POSITION);
        }
        if(DataMode == A){
            SSPSTAT &= ~(1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00100000|FrqSCK);
        }
        else if(DataMode == B){
            SSPSTAT |= (1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00100000|FrqSCK);
        }
        else if(DataMode == C){
            SSPSTAT &= ~(1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00110000|FrqSCK);
        }
        else if(DataMode == D){
            SSPSTAT |= (1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00110000|FrqSCK);
        }
    }
    else if(Role == Slave){
        TRISC3 = 1;     //C3 es entrada, SCK recibe
        if(DataMode == A){
            SSPSTAT &= ~(1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00100100);
        }
        else if(DataMode == B){
            SSPSTAT |= (1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00100100);
        }
        else if(DataMode == C){
            SSPSTAT &= ~(1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00110100);
        }
        else if(DataMode == D){
            SSPSTAT |= (1<< _SSPSTAT_CKE_POSITION);
            SSPCON |= (0b00110100);
        }
    }
}

void SPIPIC_Rwait(void){
    while(!SSPSTATbits.BF);
}

uint8_t SPIPIC_tnr(char Data){
    SSPBUF  = Data;
    SPIPIC_Rwait();
    return SSPBUF;  
}

uint8_t SPIPIC_Rec(void){
    SPIPIC_Rwait();
    return SSPBUF;
}

void SPIPIC_send(uint8_t Data){
    SSPBUF = Data;
}

void SSstart(char Port, int SlaveNum){
    if (Port == A)
    {
        PORTA &= ~(1<<SlaveNum);
    }
    else if (Port == B)
    {
        PORTB &= ~(1<<SlaveNum);
    }
    else if (Port == C)
    {
        PORTC &= ~(1<<SlaveNum);
    }
    else if (Port == D)
    {
        PORTD &= ~(1<<SlaveNum);
    }
    else if  (Port == E)
    {
        PORTE &= ~(1<<SlaveNum);
    }
}

void SSstop(char Port, int SlaveNum){
    if (Port == A)
    {
        PORTA |= (1<<SlaveNum);
    }
    else if (Port == B)
    {
        PORTB |= (1<<SlaveNum);
    }
    else if (Port == C)
    {
        PORTC |= (1<<SlaveNum);
    }
    else if (Port == D)
    {
        PORTD |= (1<<SlaveNum);
    }
    else if  (Port == E)
    {
        PORTE |= (1<<SlaveNum);
    }
}