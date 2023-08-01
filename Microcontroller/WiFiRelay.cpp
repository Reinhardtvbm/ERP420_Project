#include "WiFiRelay.h"

WiFiRelay::WiFiRelay(const char* ssid, const char* password, const char* server_ip, const int port, WiFiClient* client) : client(client) {
    Serial.println("Connecting to WiFi");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected");

    while (!client->connect(server_ip, port)) {
        Serial.println("Failed to connect to server"); 
        delay(10);
    }

    Serial.println("Connected to server :)");

    curr_time = micros();
}

void WiFiRelay::send_sensor_data(int16_t* data) {
    uint32_t new_time = micros();
    double elapsed = (double)(new_time - curr_time);
    curr_time = new_time;

    for (int i = 0; i < 9; ++i) {
        client->write(data[i]);
        client->flush();
    

        Serial.print("data rate: ");
        Serial.print(1.0 / (elapsed / 1000000.0));
        Serial.println(" Hz"); 
    }
}
