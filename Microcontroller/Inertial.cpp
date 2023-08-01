#include "Inertial.h"

void Inertial::start() {
    I2C_write_reg(INER_ADDR, INER_CTRL2_G, INER_CTRL2_G_VALUE);
    I2C_write_reg(INER_ADDR, INER_CTRL1_XL, INER_CTRL1_XL_VALUE);
    I2C_write_reg(INER_ADDR, INER_CTRL7_G, INER_CTRL7_G_VALUE);
    I2C_write_reg(INER_ADDR, INER_CTRL8_XL, INER_CTRL8_XL_VALUE);
}

void Inertial::get_values(int16_t* readings) {
    Wire.beginTransmission(INER_ADDR);
    Wire.write(INER_OUTX_L_G);
    Wire.endTransmission(false);
  
    Wire.requestFrom(INER_ADDR, 12);

    while (Wire.available() < 12) {}

    for (int i = 0; i < 6; ++i) {
        i16_bytes axis_data;

        axis_data.bytes[0] = Wire.read();
        axis_data.bytes[1] = Wire.read();

        readings[i] = axis_data.reading;
    }
}
