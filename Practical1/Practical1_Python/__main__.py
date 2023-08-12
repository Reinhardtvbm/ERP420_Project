import sys
import pyqtgraph as pg
from PyQt6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget
import numpy as np
import time
import socket
import math

HOST = "172.20.10.7"
PORT = 3000

NUM_POINTS = 1000

x_data = np.arange(0, NUM_POINTS)

MAG_FULL_SCALE = 49.152
GYRO_DPS = 2000
ACC_FULL_SCALE = 4

class RealTimePlotApp(QMainWindow):
    def __init__(self):
        super().__init__()

        self.curr_time = time.time()
        self.FPS = 0
        self.setup_ui()

        self.UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.UDPServerSocket.bind((HOST, PORT))

        self.file = open("data.txt", "w")

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
        names = ['X', 'Y', 'Z']

        for i in range(3):
            self.plot_widgets[i].addLegend(offset = (0, 0))
        # Create curve items for the plots with different colors
        self.curves = [[self.plot_widgets[i].plot(x_data, self.y_data[i][j], pen=pg.mkPen(color=colors[j]), name=names[j]) for j in range(3)] for i in range(3)]



        # Start a timer to update the plot
        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.start(3)  # Update plot every 50 milliseconds (20 FPS)

        self.plot_widgets[0].setTitle("Magnetometer")
        self.plot_widgets[0].setYRange(-MAG_FULL_SCALE / 40, MAG_FULL_SCALE / 40)
        self.plot_widgets[0].setLabel('left', 'gauss')

        self.plot_widgets[1].setTitle("Gyroscope")
        self.plot_widgets[1].setYRange(-GYRO_DPS * math.pi / 180, GYRO_DPS * math.pi / 180)
        self.plot_widgets[1].setLabel('left', 'rad/s')

        self.plot_widgets[2].setTitle("Accelerometer")
        self.plot_widgets[2].setYRange(-ACC_FULL_SCALE, ACC_FULL_SCALE)
        self.plot_widgets[2].setLabel('left', 'g')

        # Set the background color to white
        for plot_widget in self.plot_widgets:
            plot_widget.setBackground('w')

        # Set up the window
        self.setWindowTitle("Real-Time Plot")
        self.setGeometry(100, 100, 800, 600)

    def update_plot(self):
        readings = self.UDPServerSocket.recv(32)

        if len(readings) >= 18:
            readings = process_raw_readings(readings)

            self.file.write(str(readings))
            self.file.write('\n')
            # Update the plots

            for i in range(3):
                for j in range(3):  # Loop over subplots for each plot
                    y_new = readings[i][j]

                    self.y_data[i][j] = np.roll(self.y_data[i][j], -1)
                    self.y_data[i][j][-1] = y_new

                    self.curves[i][j].setData(x_data, self.y_data[i][j])

    
    def get_label_text(self, color):
        if color == (255, 0, 0):
            return "X"
        elif color == (0, 255, 0):
            return "Y"
        elif color == (0, 0, 255):
            return "Z"
        else:
            return ""

def process_raw_readings(readings):
    readings = [int.from_bytes([readings[i], readings[i + 1]], byteorder="little", signed=True) for i in range(0, 18, 2)]
    readings = [[readings[i], readings[i + 1], readings[i + 2]] for i in range(0, 9, 3)]

    for i in range(3):
        readings[0][i] *= (MAG_FULL_SCALE / (2.0 ** 15.0))
        readings[1][i] *= (GYRO_DPS / (2.0 ** 15.0)) * (math.pi / 180.0)
        readings[2][i] *= (ACC_FULL_SCALE / (2.0 ** 15.0))
    
    return readings

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RealTimePlotApp()
    window.show()
    sys.exit(app.exec())

