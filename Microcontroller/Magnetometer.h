#pragma once

#include <stdint.h>
#include "I2C_funcs.h"
#include "aux_types.h"

/// Magnetometer I2C address
#define MAG_ADDR 0x1E

/// **Registers**:

// These registers comprise a 16-bit register and represent X hard-iron offset in order to
// compensate environmental effects (data in two’s complement). These values act on the
// magnetic output data value in order to delete the environmental offset.
#define MAG_OFFSET_X_REG_L 0x45
#define MAG_OFFSET_X_REG_H 0x46
#define MAG_OFFSET_Y_REG_L 0x47
#define MAG_OFFSET_Y_REG_H 0x48
#define MAG_OFFSET_Z_REG_L 0x49
#define MAG_OFFSET_Z_REG_H 0x4A

// The identification register is used to identify the device.
#define MAG_WHO_AM_I 0x4F

// The configuration registers
#define MAG_CFG_REG_A 0x60
#define MAG_CFG_REG_B 0x61
#define MAG_CFG_REG_C 0x62

// The interrupt control registers
#define MAG_INT_CRTL_REG 0x63
#define MAG_INT_SOURCE_REG 0x64
#define MAG_INT_THS_L_REG 0x65
#define MAG_INT_THS_H_REG 0x66

// The status register is an 8-bit read-only register. This register is used to indicate device
// status.
#define MAG_STATUS_REG 0x67

// Output registers for the three axis
#define MAG_OUTX_L_REG 0x68
#define MAG_OUTX_H_REG 0x69
#define MAG_OUTY_L_REG 0x6A
#define MAG_OUTY_H_REG 0x6B
#define MAG_OUTZ_L_REG 0x6C
#define MAG_OUTZ_H_REG 0x6D

// Temperature registers
#define MAG_TEMP_OUT_L_REG 0x6E
#define MAG_TEMP_OUT_H_REG 0x6F

/// **CONFIG_VALS**:
#define MAG_CFG_REG_A_VALUE 0x8C
#define MAG_CFG_REG_B_VALUE 0x00
#define MAG_CFG_REG_C_VALUE 0x10
/// ====================================================================

class Magnetometer {
    public:
        void start();
        void get_values(int16_t* readings);
};
