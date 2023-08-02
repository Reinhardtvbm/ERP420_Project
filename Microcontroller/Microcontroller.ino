#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Magnetometer.h"
#include "Inertial.h"

const char* ssid = "vBM";
const char* password = "mightysparrow1427";
const char* server_ip = "192.168.0.178";
const int port = 3000;

WiFiUDP udp;
Magnetometer magnetometer;
Inertial inertial;
uint32_t curr_time;
boolean connected = false;

uint8_t readings[18];

void setup(void) {
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println("Connecting to WiFi");
    //register event handler
    WiFi.onEvent(WiFiEvent);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected");

    while (!connected) {
      Serial.print(".");
      delay(100);
    }

    Serial.print("\n");
  
    Wire.begin();
    magnetometer.start();
    inertial.start();

    //readings[0] = 255;
    //readings[1] = 255;
  
    curr_time = micros();

    udp.begin(WiFi.localIP(), port);
}



void loop(void) {
    uint32_t new_time = micros();
    double elapsed = (double)(new_time - curr_time) / 1000000.0;
    curr_time = new_time;

    Serial.print(1.0 / elapsed);
    Serial.println(" Hz");
  
    magnetometer.get_values(readings);
    inertial.get_values(readings + 6);

    //for (int i = 0; i < 20; ++i) {
        send_udp(readings, 18);
    //}
    
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          
          connected = true;
          Serial.println("Connected to server :)");
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}

void send_udp(uint8_t* packet, int len) {
  if (connected) {
    udp.beginPacket(server_ip, port);
    udp.write(packet, len);
    udp.endPacket();
  }
}
