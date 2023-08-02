#pragma once

#include <stdint.h>
#include "I2C_funcs.h"
#include "aux_types.h"

/// Inertial Module I2C address
#define INER_ADDR 0x6B

/// **Registers**:

/// SEE REGISTER DESCRIPTIONS ON DATASHEET PG. 48

// Enable embedded functions register (r/w)
#define INER_FUNC_CFG_ACCESS 0x01

// SDO, OCS_AUX, SDO_AUX pins pull-up enable/disable register (r/w)
#define INER_PIN_CTRL 0x02

// Sensor synchronization time frame register (r/w)
#define INER_S4S_TPH_L 0x04
#define INER_S4S_TPH_H 0x05

// Sensor synchronization resolution ratio register (r/w)
#define INER_S4S_RR 0x06

// FIFO control registers
#define INER_FIFO_CTRL1 0x07
#define INER_FIFO_CTRL2 0x08
#define INER_FIFO_CTRL3 0x09
#define INER_FIFO_CTRL4 0x0A

// Counter batch data rate registers
#define INER_COUNTER_BDR_REG1 0x0B
#define INER_COUNTER_BDR_REG2 0x0C

// INT1 pin control register (r/w)
#define INER_INT1_CTRL 0x0D

// INT2 pin control register (r/w)
#define INER_INT2_CTRL 0x0E

// WHO_AM_I register (r). This is a read-only register. Its value is fixed at 6Ch
#define INER_WHO_AM_I 0x0F

// Control registers (r/w): XL => accelerometer; G => gyroscope; C => general control
#define INER_CTRL1_XL 0x10
#define INER_CTRL2_G 0x11
#define INER_CTRL3_C 0x12
#define INER_CTRL4_C 0x13
#define INER_CTRL5_C 0x14
#define INER_CTRL6_C 0x15
#define INER_CTRL7_G 0x16
#define INER_CTRL8_XL 0x17
#define INER_CTRL9_XL 0x18
#define INER_CTRL10_C 0x19

// Source register for all interrupts (r)
#define INER_ALL_INT_SRC 0x1A

// Wake-up interrupt source register (r)
#define INER_WAKE_UP_SRC 0x1B

// Tap source register (r)
#define INER_TAP_SRC 0x1C

// Portrait, landscape, face-up and face-down source register (r)
#define INER_D6D_SRC 0x1D

// The device status register 
#define INER_STATUS_REG 0x1E

// Temperature data output registers (r)
#define INER_OUT_TEMP_L 0x20
#define INER_OUT_TEMP_H 0x21

// Data output registers: G => Gyroscope; A => Accelerometer
#define INER_OUTX_L_G 0x22
#define INER_OUTX_H_G 0x23
#define INER_OUTY_L_G 0x24
#define INER_OUTY_H_G 0x25
#define INER_OUTZ_L_G 0x26
#define INER_OUTZ_H_G 0x27
#define INER_OUTX_L_A 0x28
#define INER_OUTX_H_A 0x29
#define INER_OUTY_L_A 0x2A
#define INER_OUTY_H_A 0x2B
#define INER_OUTZ_L_A 0x2C
#define INER_OUTZ_H_A 0x2D

// Embedded function status register (r)
#define INER_EMB_FUNC_STATUS_MAINPAGE 0x35

// Finite State Machine status register (r)
#define INER_FSM_STATUS_A_MAINPAGE 0x36
#define INER_FSM_STATUS_B_MAINPAGE 0x37

// Machine Learning Core status register (r)
#define INER_MLC_STATUS_MAINPAGE 0x38

// Sensor hub source register (r)
#define INER_STATUS_MASTER_MAINPAGE 0x39

// FIFO status register 1 (r)
#define INER_FIFO_STATUS1 0x3A
#define INER_FIFO_STATUS2 0x3B

// Timestamp first data output register (r). The value is expressed as a 32-bit word and the bit resolution is 25 μs
#define INER_TIMESTAMP0 0x40
#define INER_TIMESTAMP1 0x41
#define INER_TIMESTAMP2 0x42
#define INER_TIMESTAMP3 0x43

// OIS status register (r)
#define INER_UI_STATUS_REG_OIS 0x49

// Angular rate sensor pitch axis (X/Y/Z) angular rate output register (r). The value is expressed as a 16-bit word in two’s
// complement
#define INER_UI_OUTX_L_G_OIS 0x4A
#define INER_UI_OUTX_H_G_OIS 0x4B
#define INER_UI_OUTY_L_G_OIS 0x4C
#define INER_UI_OUTY_H_G_OIS 0x4D
#define INER_UI_OUTZ_L_G_OIS 0x4E
#define INER_UI_OUTZ_H_G_OIS 0x4F
#define INER_UI_OUTX_L_A_OIS 0x50
#define INER_UI_OUTX_H_A_OIS 0x51
#define INER_UI_OUTY_L_A_OIS 0x52
#define INER_UI_OUTY_H_A_OIS 0x53
#define INER_UI_OUTZ_L_A_OIS 0x54
#define INER_UI_OUTZ_H_A_OIS 0x55

// Activity/inactivity functions, configuration of filtering, and tap recognition functions (r/w)
#define INER_TAP_CFG0 0x56
#define INER_TAP_CFG1 0x57
#define INER_TAP_CFG2 0x58

// Portrait/landscape position and tap function threshold register (r/w)
#define INER_TAP_THS_6D 0x59

// Tap recognition function setting register (r/w)
#define INER_INT_DUR2 0x5A

// Single/double-tap selection and wake-up configuration (r/w)
#define INER_WAKE_UP_THS 0x5B

// Free-fall, wakeup and sleep mode functions duration setting register (r/w)
#define INER_WAKE_UP_DUR 0x5C

// Free-fall function duration setting register (r/w)
#define INER_FREE_FALL 0x5D

// Functions routing on INT1/2 registers (r/w)
#define INER_MD1_CFG 0x5E
#define INER_MD2_CFG 0x5F

// S4S Master command register (r/w)
#define INER_S4S_ST_CMD_CODE 0x60

// S4S DT register (r/w)
#define INER_S4S_DT_REG 0x61

// I3C_BUS_AVB register (r/w)
#define INER_I3C_BUS_AVB 0x62

// Internal frequency register (r)
#define INER_INTERNAL_FREQ_FINE 0x63

// OIS interrupt configuration register and accelerometer self-test enable setting
#define INER_UI_INT_OIS 0x6F

// OIS configuration registers
#define INER_UI_CTRL1_OIS 0x70
#define INER_UI_CTRL2_OIS 0x71
#define INER_UI_CTRL3_OIS 0x72

// Accelerometer X/Y/Z-axis user offset correction (r/w)
#define INER_X_OFS_USR 0x73
#define INER_Y_OFS_USR 0x74
#define INER_Z_OFS_USR 0x75

// FIFO tag register (r)
#define INER_FIFO_DATA_OUT_TAG 0x78

// FIFO data output (r)
#define INER_FIFO_DATA_OUT_X_L 0x79
#define INER_FIFO_DATA_OUT_X_H 0x7A
#define INER_FIFO_DATA_OUT_Y_L 0x7B
#define INER_FIFO_DATA_OUT_Y_H 0x7C
#define INER_FIFO_DATA_OUT_Z_L 0x7D
#define INER_FIFO_DATA_OUT_Z3_H 0x7E

/// **CONFIG_VALS**:
#define INER_CTRL2_G_VALUE 0b10001100 
#define INER_CTRL1_XL_VALUE 0b10001010
#define INER_CTRL7_G_VALUE 0b00000000
#define INER_CTRL8_XL_VALUE 0b00001001 
/// ====================================================================

class Inertial {
    public:
        void start();
        void get_values(uint8_t* readings);
};
