#include <WiFi.h>

class WiFiRelay {
    public:
        WiFiRelay(const char* ssid, const char* password, const char* server_ip, const int port);
        void send_sensor_data(double* data);

    private:
        WiFiClient client;
};
