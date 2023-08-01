#ifndef I2C_FUNCS_H
#define I2C_FUNCS_H

#include <stdint.h>
#include <Wire.h>

void I2C_write_reg(uint8_t device_address, uint8_t register_address, uint8_t new_value);
void I2C_read_reg(uint8_t device_address, uint8_t register_address, uint8_t* buffer, int len);

#endif
