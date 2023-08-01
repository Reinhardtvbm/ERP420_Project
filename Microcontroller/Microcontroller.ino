#include <Wire.h>
#include "Magnetometer.h"
#include "Inertial.h"
#include "WiFiRelay.h"

const char* ssid = "RPC";
const char* password = "boopboop";
const char* server_ip = "";
const int port = 3000;

WiFiClient client;

WiFiRelay wifi(ssid, password, server_ip, port, &client);
Magnetometer magnetometer;
Inertial inertial;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin();
  magnetometer.start();
  inertial.start();
}

int16_t readings[9];

void loop(void) {
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

  wifi.send_sensor_data(readings);

}
