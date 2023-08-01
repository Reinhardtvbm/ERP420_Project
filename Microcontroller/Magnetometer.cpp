#include "Magnetometer.h"

// starts the magnetomoter by writing the correct values to the config registers
void Magnetometer::start() {
  I2C_write_reg(MAG_ADDR, MAG_CFG_REG_A, MAG_CFG_REG_A_VALUE);
  I2C_write_reg(MAG_ADDR, MAG_CFG_REG_B, MAG_CFG_REG_B_VALUE);
  I2C_write_reg(MAG_ADDR, MAG_CFG_REG_C, MAG_CFG_REG_C_VALUE);
}

void Magnetometer::get_values(int16_t* readings) {
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(MAG_OUTX_L_REG);
  Wire.endTransmission(false);

  Wire.requestFrom(MAG_ADDR, 6);

  while (Wire.available() < 6) {}

  for (int i = 0; i < 3; ++i) {
    i16_bytes axis_data;

    axis_data.bytes[0] = Wire.read();
    axis_data.bytes[1] = Wire.read();

    readings[i] = axis_data.reading;
  }
}
