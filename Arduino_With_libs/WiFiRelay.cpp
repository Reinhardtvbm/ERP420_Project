#include <Arduino.h>
#include "WiFiRelay.h"

WiFiRelay::WiFiRelay(const char* ssid, const char* password, const char* server_ip, const int port) {
    
}

void WiFiRelay::send_sensor_data(double* data) {
  for (int i = 0; i < 9; ++i) {
    uint32_t start = micros();
    
    client.write(data[i]);
    client.flush();
    
    uint32_t end = micros();

    Serial.print("data rate: ");
    Serial.print(1 / ((double)(end - start) / 1000000.0));
    Serial.println(" Hz"); 
  }
}
