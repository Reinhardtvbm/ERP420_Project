#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <cstdint>
#include <math.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "constants.h"

class UDPServer {
    public:
        UDPServer();
        void receive(double[], double);

    private:
        int socket_descriptor, client_struct_length;
        struct sockaddr_in client_addr;
};

#endif // UDPSERVER_H
