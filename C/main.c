#include "time.h"
#include "GLFW/glfw3.h"
#include <math.h>
#include <GL/gl.h>
#include <alloca.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

const double plot_x_offset = 10.0;
const double plot_y_offset = 30.0;

void draw_plot(double* x_vals, double* y_vals, int plot_size, double R, double G, double B, double y_offset) {
    glColor3f(R, G, B);
    glBegin(GL_LINES);

    for (int i = 0; i < plot_size; ++i) {
        glVertex2f(x_vals[i], y_vals[i] + y_offset);
    }

    glEnd();
}

void drawGraph(double* x_vals, double** readings, int plot_size) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    for (int i = 0; i < 9; i += 3) {
        draw_plot(x_vals, readings[i], plot_size, 2.0, 0.0, 0.0, 10.0 + 12.0 * i);
        draw_plot(x_vals, readings[i + 1], plot_size, 0.0, 1.0, 0.0, 10.0 + 12.0 * i);
        draw_plot(x_vals, readings[i + 2], plot_size, 0.0, 0.0, 1.0, 10.0 + 12.0 * i);
    }

    glFlush();
}

int main() {
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

    // make sure the GLFW library initialises properly
    if (!glfwInit()) {
        return -1;
    }

    printf("creating window\n");

    // create a new window
    GLFWwindow* window = glfwCreateWindow(1200, 1000, "Real Time plot", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(-10.0, 10.0, 0.0, 100.0, -1.0, 1.0); // Set the coordinate system

    double FPS = 0.0;
    
    double x[1000];
    double** y;
    
    printf("allocating arrays\n");

    y = (double**)malloc(sizeof(double*) * 9);

    for (int i = 0; i < 1000; i++) {
        x[i] = ((double)i / 50.0) - 10.0;

    }

    for (int i = 0; i < 9; ++i) {
        y[i] = (double*)malloc(sizeof(double) * 1000);
        for (int j = 0; j < 1000; ++j) {
            y[i][j] = 0.0;
        }
    }

    printf("starting\n");

    int count = 1000;
    double start = glfwGetTime();

    double max[3] = {0.0, 0.0, 0.0};

    int16_t readings[9];

    while (!glfwWindowShouldClose(window)) {
        
        drawGraph(x, y, 1000);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (recvfrom(socket_desc, readings_raw, 18, 0,
                    (struct sockaddr*)&client_addr, (socklen_t*)&client_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }

        //printf("Received message from IP: %s and port: %i\n",
        //        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        

        printf("\n[");
        for (int i = 0; i < 18; i += 2) {
            int16_t reading = ((int16_t)readings_raw[i + 1] << 8) | readings_raw[i];

            if (readings[i/2] != reading) {
                readings[i/2] = reading;
            } else {
                printf("not new\n");
                continue;
            }


            if (fabs((double)reading) > max[i/6]) {
                max[i/6] = abs(reading);
            }

            printf("%d\t", reading);
        }
        printf("]\n\n");

        double new_time = clock();
        double cpu_time_used = (new_time - start) / CLOCKS_PER_SEC;
        start = new_time;
        
        FPS = 0.8 * FPS + (0.2 * (1.0 / cpu_time_used));
        
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 999; ++j) {
                y[i][j] = y[i][j+1];
            }
            
            y[i][999] = (double)readings[i] * 10.0 / max[i/3];
        }
        
        printf("%d FPS\t%f\t%f\t%f\n", (int)FPS, max[0], max[1], max[2]);

    }

    glfwTerminate();
    return 0;
}

