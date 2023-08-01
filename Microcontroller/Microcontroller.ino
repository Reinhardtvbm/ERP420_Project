#include <Wire.h>
#include "Magnetometer.h"
#include "Inertial.h"

Magnetometer magnetometer;
Inertial inertial;

uint32_t time_prev = micros();

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin();
  magnetometer.start();
  inertial.start();
}

void loop(void) {
  uint32_t time_curr = micros();

  Serial.println(1.0 / ((double)(time_curr - time_prev) / 1000000.0));
  time_prev = time_curr;
  // sample data at 1000Hz
//  if (time_curr - time_prev < 1000) {
//    Serial.println("waiting");
//    return;
//  } else {
//    time_prev = time_curr;
//  }

  int16_t readings[9];

  magnetometer.get_values(readings);

//  Serial.print("M_X:\t");
//  Serial.print(readings[0]);
//  Serial.print("\tM_Y:\t");
//  Serial.print(readings[1]);
//  Serial.print("\tM_Z:\t");
//  Serial.print(readings[2]);
  
  inertial.get_values(readings + 3);
  
//  Serial.print("\t||\tG_X:\t");
//  Serial.print(readings[3]);
//  Serial.print("\tG_Y:\t");
//  Serial.print(readings[4]);
//  Serial.print("\tG_Z:\t");
//  Serial.print(readings[5]);
//
//  Serial.print("\t||\tA_X:\t");
//  Serial.print(readings[6]);
//  Serial.print("\tA_Y:\t");
//  Serial.print(readings[7]);
//  Serial.print("\tA_Z:\t");
//  Serial.println(readings[8]);
}
