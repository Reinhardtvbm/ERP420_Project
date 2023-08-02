import sys
import pyqtgraph as pg
from PyQt6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget
import numpy as np
import time
import socket

HOST = "192.168.0.178"
PORT = 3000

NUM_POINTS = 1000

x_data = np.arange(0, NUM_POINTS)

class RealTimePlotApp(QMainWindow):
    def __init__(self):
        super().__init__()

        self.curr_time = time.time()
        self.FPS = 0
        self.setup_ui()

        self.UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.UDPServerSocket.bind((HOST, PORT))

    def setup_ui(self):
        # Create a central widget and a QVBoxLayout to hold the plot
        central_widget = QWidget(self)
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)

        # Create the 3 plotwidgets with 3 subplots each
        self.plot_widgets = [pg.PlotWidget() for _ in range(3)]

        for i in range(3):
            # Divide the plot into three regions (3 rows, 1 column)
            layout.addWidget(self.plot_widgets[i])

        # Initialize data for the 3 plots (3 subplots for each)
        self.y_data = [[np.zeros(NUM_POINTS) for _ in range(3)] for _ in range(3)]

        # Define colors for each subplot
        colors = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]

        # Create curve items for the plots with different colors
        self.curves = [[self.plot_widgets[i].plot(x_data, self.y_data[i][j], pen=pg.mkPen(color=colors[j])) for j in range(3)] for i in range(3)]

        # Start a timer to update the plot
        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.start(0)  # Update plot every 50 milliseconds (20 FPS)

        # Set the background color to white
        for plot_widget in self.plot_widgets:
            plot_widget.setBackground('w')

        # Set up the window
        self.setWindowTitle("Real-Time Plot")
        self.setGeometry(100, 100, 800, 600)

    def update_plot(self):
        new_time = time.time()

        elapsed = new_time - self.curr_time
        self.curr_time = new_time

        new_FPS = 1/elapsed

        self.FPS = 0.95*self.FPS + 0.05*new_FPS

        readings = self.UDPServerSocket.recv(32)

        if len(readings) >= 18:
            readings = [[readings[i], readings[i + 1]] for i in range(0, 18, 2)]

            # Update the plots
            for i in range(3):
                for j in range(3):  # Loop over subplots for each plot
                    y_new = int.from_bytes(readings[(i * 3) + j], byteorder='little', signed=True)

                    self.y_data[i][j] = np.roll(self.y_data[i][j], -1)
                    self.y_data[i][j][-1] = y_new

                    self.curves[i][j].setData(x_data, self.y_data[i][j])

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RealTimePlotApp()
    window.show()
    sys.exit(app.exec())

