#pragma once

#include <stdint.h>

/// union for converting the 2 bytes from each output register into its 
/// corresponding 16-bit integer
typedef union sensor_data {
  int16_t reading;
  uint8_t bytes[2];
} i16_bytes;