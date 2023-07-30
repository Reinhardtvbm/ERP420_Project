#include <WiFi.h>

const char* ssid     = "R_iPhone"; // Change this to your WiFi SSID
const char* password = "boopboop"; // Change this to your WiFi password

const char* server_ip = "172.20.10.7"; // Change this to the ip of server on laptop
const int port = 3000; // This should not be changed (server port should be the same)

void setup() {
    Serial.begin(115200);
    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;

  if (!client.connect(server_ip, port)) {
    Serial.println("connection_failed");
    return;
  }

  Serial.println("connected to server");
  
  int data_point[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  // send the 9 data points as fast as possible
  while (1) {
    for (int i = 0; i < 9; ++i) {
      uint32_t start = micros();
      client.write(data_point[i]++);
      
      if (data_point[i] == 256) {
        data_point[i] = 0; 
      }

      client.flush();
      uint32_t end = micros();

       Serial.print("data rate: ");
       Serial.print(1 / ((double)(end - start) / 1000000.0));
       Serial.println(" Hz"); 
    }
  }
}
