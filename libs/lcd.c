#include <msp430.h>
#include <stdio.h>
#include "i2c.h"
#include "lcd.h"

uint8_t pos = 0;

void lcdInit(void){                             // Incialize LCD in 4 bits mode.
    // 8 bits mode.
    lcdWrNib(0x3, 0);
    lcdWrNib(0x3, 0);
    lcdWrNib(0x3, 0);

    // 4 bits mode.
    lcdWrNib(0x2, 0);

    // Number of lines.
    lcdWrByte(0x28, 0);

    // Enable display and cursor.
    lcdWrByte(0x0F, 0);

    // Clear display.
    lcdWrByte(0x01, 0);
}

void lcdWrNib(uint8_t nib, uint8_t isChar){     // Write nibble function.
    // Put nibble on most significant part of LCD's channels, so the i2c can send the data to it.
    nib = nib << 4;

    // Sending nibble to LCD by enable pulse.
    i2cSend(0x27, nib | BIT3 |  0   | isChar);
    i2cSend(0x27, nib | BIT3 | BIT2 | isChar);
    i2cSend(0x27, nib | BIT3 |  0   | isChar);
}

void lcdWrByte(uint8_t byte, uint8_t isChar){   // Function to write a byte on
    lcdWrNib(byte >> 4, isChar);                // Send MSB first.
    lcdWrNib(byte & 0xF, isChar);               // Send LSB.
}

void lcdPrint(char * str){                      // Print string into LCD.
    while(*str){
        lcdWrByte(*str++, 1);
        __delay_cycles(50000);                  // A little delay so we can see character by character being written onto the LCD.
    }

}

void lcdPrintFast(char * str){                      // Print string into LCD without delay.
    while(*str){
        lcdWrByte(*str++, 1);
    }

}

void lcdPrintInstr(char * str){
    while(*str){
        if (pos == 16) {
            lcdWrByte(0xC0, 0);
            pos = 0;
        }
        lcdWrByte(*str++, 1);
        __delay_cycles(50000);
        pos += 1;
    }

}

void lcdNextLine(void){                         // Jump to next LCD's line.
    lcdWrByte(0xC0, 0);
}

void lcdResetDisplay(void){                     // Reset LCD display.
    lcdWrByte(0x80, 0);                         // Change cursor address to first LCD's DDRAM address.
    lcdWrByte(0x01, 0);                         // Clear LCD display.
}
