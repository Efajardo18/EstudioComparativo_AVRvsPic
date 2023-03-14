/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ
#endif

#define	XC_HEADER_TEMPLATE_H
#define _8M   7
#define _4M   6
#define _2M   5
#define _1M   4
#define _500K 3
#define _250K 2
#define _125K 1
#define _31K  0

#include <xc.h> // include processor files - each processor file is guarded.  

void OSC_set(char FreqClockSelect, char InternalOsc){
    if (FreqClockSelect == _8M){
        OSCCONbits.IRCF = 7;        
    }
    else if (FreqClockSelect == _4M){
        OSCCONbits.IRCF = 6;
    }
    else if (FreqClockSelect == _2M){
        OSCCONbits.IRCF = 5;
    }
    else if (FreqClockSelect == _1M){
        OSCCONbits.IRCF = 4;
    }
    else if (FreqClockSelect == _500K){
        OSCCONbits.IRCF = 3;
    }
    else if (FreqClockSelect == _250K){
        OSCCONbits.IRCF = 2;
    }
    else if (FreqClockSelect == _125K){
        OSCCONbits.IRCF = 1;
    }
    else if (FreqClockSelect == _31K){
        OSCCONbits.IRCF = 0;
    }
    if (InternalOsc == 1){
        OSCCON |= (1<< _OSCCON_SCS_POSITION);
    }
    else if (InternalOsc == 0){
        OSCCON &= ~(1<< _OSCCON_SCS_POSITION);
    }
}

