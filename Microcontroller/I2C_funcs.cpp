#include "I2C_funcs.h"

void I2C_write_reg(uint8_t device_address, uint8_t register_address, uint8_t new_value) {
  Wire.beginTransmission(device_address);
  Wire.write(register_address);
  Wire.write(new_value);
  Wire.endTransmission();
}

void I2C_read_reg(uint8_t device_address, uint8_t register_address, uint8_t* buffer, int len) {
  Wire.beginTransmission(device_address);
  Wire.write(register_address);
  Wire.endTransmission(false);

  Wire.requestFrom(device_address, len);

  while (Wire.available() < len) {}
  
  for (int i = 0; i < len; ++i) {
    buffer[i] = Wire.read();
  }
}