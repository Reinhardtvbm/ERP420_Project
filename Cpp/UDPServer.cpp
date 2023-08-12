#include "UDPServer.h"

UDPServer::UDPServer() {
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    int client_struct_length = sizeof(client_addr);

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(socket_desc < 0){
        std::cout << "Error while creating socket\n";
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.178");

    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        std::cout << "Couldn't bind to the port\n";
    }
    std::cout << "Done binding\n";

    this->socket_descriptor = socket_desc;
    this->client_struct_length = client_struct_length;
    this->client_addr =client_addr;
}

void UDPServer::receive(double readings[], double fps) {
    uint8_t readings_raw[18];

    if (recvfrom(this->socket_descriptor, readings_raw, 18, 0,
                 (struct sockaddr*)&(this->client_addr), (socklen_t*)&this->client_struct_length) < 0){
        printf("Couldn't receive\n");
    }

    int j = 0;

    const double scale_bin = pow(2.0, 15.0);

    std::cout << "[";
    for (int i = 0; i < 18; i += 2) {
        int16_t reading = ((int16_t)readings_raw[i + 1] << 8) | readings_raw[i];
        readings[i/2] = reading;

        switch (j / 3) {
            case 0: readings[i/2] = reading * (MAG_FULL_SCALE / scale_bin);
            case 1: readings[i/2] = reading * ((GYRO_DPS / scale_bin) * M_PI / 180.0);
            case 2: readings[i/2] = reading * (ACC_FULL_SCALE / scale_bin);
        }

        std::cout << reading << '\t';

        j++;
    }
    std::cout << "]\n" << fps << " FPS\n" << "\r\r";
}
