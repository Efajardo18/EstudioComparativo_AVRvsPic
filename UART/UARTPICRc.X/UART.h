/* 
 * File:   UART.h
 * Author: Olifa
 *
 * Created on 18 de agosto de 2022, 02:19 PM
 */
#define Sync    1
#define Async   0


#include <math.h>

void UART_set(int baud, char sync, char 16BitBaud, char HighSpeed)
{
    if(Sync == Async){
        if((16BitBaud == 1)&&(HighSpeed == 1)){
            PeriodBaud = F_CPU/(baud *4) - 1;
        }
        else if((16BitBaud == 1)&&(HighSpeed == 0)){
            PeriodBaud = F_CPU/(baud *16) - 1;
        }
        else if((16BitBaud == 0)&&(HighSpeed == 1)){
            PeriodBaud = F_CPU/(baud *16) - 1;
        }
    }
}

