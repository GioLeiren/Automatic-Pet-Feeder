#ifndef __I2C_H
#define __I2C_H
#include <stdint.h>

void i2cConfig();

uint8_t i2cSend(uint8_t addr, uint8_t data);


#endif
