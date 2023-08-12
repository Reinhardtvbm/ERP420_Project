#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Magnetometer.h"
#include "Inertial.h"

const char* ssid = "R_iPhone";
const char* password = "boopboop";
const char* server_ip = "172.20.10.7";
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
    Wire.setClock(1000000UL);
    
    magnetometer.start();
    inertial.start();
  
    curr_time = micros();

    udp.begin(WiFi.localIP(), port);
}

double send_delay = 1 / 200.0; // send at a rate of 200 Hz

void loop(void) {
    uint32_t new_time = micros();
    double elapsed = (double)(new_time - curr_time) / 1000000.0;

    if (elapsed > send_delay) {
        Serial.print(1.0 / elapsed);
        Serial.println(" Hz");
    
        curr_time = new_time;
    
        magnetometer.get_values(readings);
        inertial.get_values(readings + 6);

        send_udp(readings, 18);

//        for (int i = 0; i < 18; ++i) {
//          Serial.print(readings[i]);
//          Serial.print(", ");
//        }
//
//        Serial.println();
    }   
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
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
