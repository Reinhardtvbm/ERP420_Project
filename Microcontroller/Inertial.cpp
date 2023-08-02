#include "Inertial.h"

void Inertial::start() {
    I2C_write_reg(INER_ADDR, INER_CTRL2_G, INER_CTRL2_G_VALUE);
    I2C_write_reg(INER_ADDR, INER_CTRL1_XL, INER_CTRL1_XL_VALUE);
    I2C_write_reg(INER_ADDR, INER_CTRL7_G, INER_CTRL7_G_VALUE);
    I2C_write_reg(INER_ADDR, INER_CTRL8_XL, INER_CTRL8_XL_VALUE);
}

void Inertial::get_values(uint8_t* readings) {
    I2C_read_reg(INER_ADDR, INER_OUTX_L_G, readings, 12);
}
