#ifndef __LCD_H
#define __LCD_H
#include <stdint.h>

void lcdInit(void);
void lcdWrNib(uint8_t nib, uint8_t isChar);
void lcdWrByte(uint8_t byte, uint8_t isChar);
void lcdPrint(char * str);
void lcdPrintFast(char * str);
void lcdPrintInstr(char * str);
void lcdNextLine(void);
void lcdResetDisplay(void);

#endif
