#include <WiFi.h>
#include "WiFiRelay.h"

const char* ssid     = "R_iPhone"; // Change this to your WiFi SSID
const char* password = "boopboop"; // Change this to your WiFi password

const char* server_ip = "172.20.10.7"; // Change this to the ip of server on laptop
const int port = 3000; // This should not be changed (server port should be the same)

WiFiRelay relay(ssid, password, server_ip, port);

void setup() {
    Serial.begin(115200);
    while(!Serial){delay(100);}
}

void loop() {
  double data_points[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  relay.send_sensor_data(data_points);  
}
