#include <msp430.h>
#include <stdint.h>

void i2cConfig()
{
    // Master configuration.
    UCB0CTL1 = UCSWRST;
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB0CTL1 |= UCSSEL__SMCLK;
    UCB0BRW = 10;
    P3SEL |= BIT0 | BIT1;
    P3REN |= BIT0 | BIT1;
    P3OUT |= BIT0 | BIT1;
    UCB0CTL1 &= ~UCSWRST;
}

uint8_t i2cSend(uint8_t addr, uint8_t data)
{
    UCB0I2CSA = addr;
    UCB0CTL1 |= UCTR | UCTXSTT;
    UCB0TXBUF = data;
    while (UCB0CTL1 & UCTXSTT);
    UCB0CTL1 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);
    if (UCB0IFG & UCNACKIFG)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
