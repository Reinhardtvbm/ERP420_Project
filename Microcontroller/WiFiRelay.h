#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <stdint.h>

class WiFiRelay {
    public:
        WiFiRelay(const char* ssid, const char* password, const char* server_ip, const int port, WiFiClient* client);
        void send_sensor_data(int16_t* data);

    private:
        WiFiClient* client;
        uint32_t curr_time;
};
