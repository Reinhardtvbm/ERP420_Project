#include "time.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

const double plot_x_offset = 10.0;
const double plot_y_offset = 10.0;

void drawGraph(double* x_vals, double* y_vals, int plot_size) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glColor3f(1.0, 0.0, 0.0);

    // Draw the graph points
    glBegin(GL_POINTS);

    for (int i = 0; i < plot_size; ++i) {
        glVertex2f(x_vals[i], y_vals[i] + plot_y_offset);
    }

    glEnd();

    glFlush();
}

int main() {
    // make sure the GLFW library initialises properly
    if (!glfwInit()) {
        return -1;
    }

    // create a new window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Graph of y = x^2", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(-10.0, 10.0, 0.0, 100.0, -1.0, 1.0); // Set the coordinate system

    double FPS = 0.0;
    
    double x[1000];
    double y[1000];

    for (int i = 0; i < 1000; i++) {
        x[i] = ((double)i / 50.0) - 10.0;
        y[i] = 0.0;
    }

    int count = 1000;

    while (!glfwWindowShouldClose(window)) {
        clock_t start = clock();
        
        drawGraph(x, y, 1000);
        glfwSwapBuffers(window);
        glfwPollEvents();

        double cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
        
        FPS = 0.8 * FPS + (0.2 * (1.0 / cpu_time_used));

        for (int i = 0; i < 999; ++i) {
            y[i] = y[i+1];
        }

        y[999] = FPS / 50.0;
        
        printf("%f FPS\n", FPS);
    }

    glfwTerminate();
    return 0;
}

