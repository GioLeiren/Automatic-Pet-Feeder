#include <msp430.h>
#include <stdint.h>
#include "uart.h"

void uartInit(void){        //Initialize UART with 9600 baudrate.
    UCA1CTL1 = UCSWRST;
    UCA1CTL0 = UCMODE_0;
    UCA1CTL1 |= UCSSEL_2; UCA1BRW = 6;
    UCA1MCTL = UCOS16 | UCBRF_13;
    P4SEL |= (BIT4|BIT5);
    UCA1CTL1 &= ~UCSWRST;
    UCA1IE |= UCRXIE;
}
