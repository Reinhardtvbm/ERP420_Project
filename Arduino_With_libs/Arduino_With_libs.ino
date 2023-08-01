#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include "WiFiRelay.h"

const char* ssid     = "RPC";       // The name of my PC's hotspot
const char* password = "boopboop";  // hotspot password

const char* server_ip = "192.168.137.1";  // IP of server on PC
const int port = 3000;                    // Port that server is hosted on

// to connect to the server
WiFiClient client;

// Sensor gives back 32bit integers
// A union stores multiple data types in one memory location, so we can now address each byte in the 
// 32 bit int to send it to the server one byte at a time 
typedef union _data {
  int32_t   i32;
  uint8_t   bytes[4];
} int32_bytes;

// Magnetometer ======================================================================
// Slave Address
const uint8_t MAG_SLAVE_ADDR = 0b0011110;

// Internal Registers:
const uint8_t OUTX_L_REG = 0x68;
const uint8_t OUTX_H_REG = 0x69;
const uint8_t OUTY_L_REG = 0x6A;
const uint8_t OUTY_H_REG = 0x6B;
const uint8_t OUTZ_L_REG = 0x6C;
const uint8_t OUTZ_H_REG = 0x6D;
// ===================================================================================

void setup() {
    Wire.begin();
    Serial.begin(9600);

    
    
    while(!Serial){delay(100);}

    
    
    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // connect to the WiFi
    WiFi.begin(ssid, password);

    // wait for the MCU to connect to the WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // connect to the server on the PC
    while (!client.connect(server_ip, port)) {
      Serial.println("connection_failed");
    }

    Serial.println("connected to server");

//    Wire.beginTransmission(MAG_SLAVE_ADDR);
//    Wire.write(0x62);
//    Wire.write(0b);
//    Wire.endTransmission();
}

void loop() {
  // test data to send to the server
  int32_t data_points[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  for (int sensor_type = 0; sensor_type < 3; ++sensor_type) {
    // get three sets of readings from only magnetometer for now
    int32_t readings[3];

    read_magnetometer(readings);

    for (int i = 0; i < 3; ++i) {
      int32_bytes data_point;
      data_point.i32 = readings[i];    

      // send each byte in the 32 bit int
      client.write(data_point.bytes[0]);
      client.write(data_point.bytes[1]);
      client.write(data_point.bytes[2]);
      client.write(data_point.bytes[3]);

      // wait for all the bytes to be sent to the server
      client.flush();
    }
  }
}

typedef union i16_union
{
  int16_t i16;
  uint8_t bytes[2];
} i16_bytes;

uint8_t address = 76;

// will put the X, Y, and Z magnetometer readings in `readings` (in that order)
void read_magnetometer(int32_t* readings) {
  Wire.beginTransmission(MAG_SLAVE_ADDR);

  Wire.write(address++);
  
  Wire.endTransmission(false);

  Wire.requestFrom(MAG_SLAVE_ADDR, 6);

  Serial.print("address: ");
  Serial.println(address);
  
  if (Wire.available() <= 6) {
    for (int i = 0; i < 3; ++i) {
      i16_bytes axis_reading;

      axis_reading.bytes[0] = Wire.read();
      axis_reading.bytes[1] = Wire.read();
      
      Serial.print(axis_reading.bytes[0]);
      Serial.print(", ");
      Serial.println(axis_reading.bytes[1]);
      
      readings[i] = (int32_t)(axis_reading.i16);
    }
  }   

  if (address == 111) {
    address = 76;
  }

  delay(50);
}
