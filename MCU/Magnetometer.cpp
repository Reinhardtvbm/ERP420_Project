#include "Magnetometer.h"

// starts the magnetomoter by writing the correct values to the config registers
void Magnetometer::start() {
  I2C_write_reg(MAG_ADDR, MAG_CFG_REG_A, MAG_CFG_REG_A_VALUE);
  I2C_write_reg(MAG_ADDR, MAG_CFG_REG_B, MAG_CFG_REG_B_VALUE);
  I2C_write_reg(MAG_ADDR, MAG_CFG_REG_C, MAG_CFG_REG_C_VALUE);
}

void Magnetometer::get_values(uint8_t* readings) {
  I2C_read_reg(MAG_ADDR, MAG_OUTX_L_REG, readings, 6);
}
