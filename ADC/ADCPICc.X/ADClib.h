#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

void GOCHK(void){
    if(ADCON0bits.GO==0){
            ADCON0bits.GO=1;
        }
}