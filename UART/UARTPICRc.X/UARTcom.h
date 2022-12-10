/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#define	XC_HEADER_TEMPLATE_H
#define Sync    1
#define Async   0
#define Cont    1
#define Sing    0

#include <xc.h> 
#include <math.h>

void UART_BDset(int baud, char SyncSel, char BitBaudSize, char HighSpeed)
{
    if(SyncSel == Async){
        if((BitBaudSize == 1)&&(HighSpeed == 1)){
            BAUDCTL |= (1 << _BAUDCTL_BRG16_POSITION);
            TXSTA   = (TXSTA | (1 << _TXSTA_BRGH_POSITION)) & ~(1 << _TXSTA_SYNC_POSITION);
            uint16_t PeriodBaud = (uint16_t) (((_XTAL_FREQ/(baud))/ (4)) -1);
            SPBRGH  = (uint8_t) (PeriodBaud >> 8);
            SPBRG   = (uint8_t) (PeriodBaud);
            RCSTA   |= (1<< _RCSTA_SPEN_POSITION);
        }
        else if((BitBaudSize == 1)&&(HighSpeed == 0)){
            BAUDCTL |= (1 << _BAUDCTL_BRG16_POSITION);
            TXSTA   &= ~(1 << _TXSTA_BRGH_POSITION) & ~(1 << _TXSTA_SYNC_POSITION);
            uint16_t PeriodBaud = (uint16_t) (((_XTAL_FREQ/(baud))/ (16)) -1);
            SPBRGH  = (uint8_t) (PeriodBaud >> 8);
            SPBRG   = (uint8_t) (PeriodBaud);
            RCSTA   |= (1<< _RCSTA_SPEN_POSITION);
        }
        else if((BitBaudSize == 0)&&(HighSpeed == 1)){
            BAUDCTL &= ~(1 << _BAUDCTL_BRG16_POSITION);
            TXSTA   = (TXSTA | (1 << _TXSTA_BRGH_POSITION))& ~(1 << _TXSTA_SYNC_POSITION);
            uint16_t PeriodBaud = (uint16_t) (((_XTAL_FREQ/(baud))/ (16)) -1);
            SPBRG   = (uint8_t) (PeriodBaud);
            RCSTA   |= (1<< _RCSTA_SPEN_POSITION);
        }
        else if((BitBaudSize == 0)&&(HighSpeed == 0)){
            BAUDCTL &= ~(1 << _BAUDCTL_BRG16_POSITION);
            TXSTA   &= ~(1 << _TXSTA_BRGH_POSITION) & ~(1 << _TXSTA_SYNC_POSITION);
            uint16_t PeriodBaud = (uint16_t) (((_XTAL_FREQ/(baud))/ (64)) -1);
            SPBRG   = (uint8_t) (PeriodBaud);
            RCSTA   |= (1<< _RCSTA_SPEN_POSITION);
        }
    }
    else if(SyncSel == Sync){
        if  (BitBaudSize == 1){
            BAUDCTL |= (1 << _BAUDCTL_BRG16_POSITION);
        }
        else{
            BAUDCTL &= ~(1 << _BAUDCTL_BRG16_POSITION);
        }
        TXSTA   &= ~(1 << _TXSTA_SYNC_POSITION);
        uint16_t PeriodBaud = (uint16_t) (((_XTAL_FREQ/(baud))/ (4)) -1);
        SPBRGH  = (uint8_t) (PeriodBaud >> 8);
        SPBRG   = (uint8_t) (PeriodBaud);
        RCSTA   |= (1<< _RCSTA_SPEN_POSITION);
    }
    return;
}
void UART_RXset(char ContSingRx, char InterruptEnable){
    if (ContSingRx == Sing){
        RCSTA   =(RCSTA | (1 << _RCSTA_SREN_POSITION)) & ~(1 << _RCSTA_CREN_POSITION); 
    }
    else if (ContSingRx == Cont){
        RCSTA   |= (1 << _RCSTA_CREN_POSITION);
    }
    if  (InterruptEnable == 1){
        INTCON  |= (1 << _INTCON_GIE_POSITION)|(1 << _INTCON_PEIE_POSITION);
        PIE1    |= (1<< _PIE1_RCIE_POSITION);
    }
    else if (InterruptEnable == 0){
        INTCON  &= ~(1 << _INTCON_GIE_POSITION)& ~(1 << _INTCON_PEIE_POSITION);
        PIE1    &= ~(1<< _PIE1_RCIE_POSITION);
    }
    return;
}

uint8_t UART_Rec(void){
    if(PIR1bits.RCIF == 1){
        return RCREG;
    }
} 

