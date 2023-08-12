#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    uint8_t readings_raw[18];
    int client_struct_length = sizeof(client_addr);
    
    memset(readings_raw, 0, sizeof(readings_raw));
    
    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.178");
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    printf("Listening for incoming messages...\n\n");
    
    // Receive client's message:
    while (1) {
        if (recvfrom(socket_desc, readings_raw, 18, 0,
                    (struct sockaddr*)&client_addr, (socklen_t*)&client_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }

        //printf("Received message from IP: %s and port: %i\n",
        //        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        printf("[");
        for (int i = 0; i < 18; i += 2) {
            int16_t reading = ((int16_t)readings_raw[i + 1] << 8) | readings_raw[i];
            printf("%d\t", reading);
        }
        printf("\n");
    }

    
    
    return 0;
}
