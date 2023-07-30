import sys
import pyqtgraph as pg
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPen
import numpy as np
import time


NUM_POINTS = 1000

class RealTimePlotApp(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.curr_time = time.time()
        self.FPS = 0
        self.setup_ui()

    def setup_ui(self):
        # Create a central widget and a QVBoxLayout to hold the plot
        central_widget = QWidget(self)
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)

        # Create the 3 plotwidgets
        self.plot_widgets = [pg.PlotWidget() for _ in range(3)]

        for i in range(3):
            layout.addWidget(self.plot_widgets[i])

        # Initialize data for the 3 plots
        self.x_data = [np.arange(0, NUM_POINTS) for _ in range(3)]
        self.y_data = [np.zeros(NUM_POINTS) for _ in range(3)]

        # Create curve items for the plots
        self.curves = [self.plot_widgets[i].plot(self.x_data[i], self.y_data[i]) for i in range(3)]

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

        # update the plots
        for i in range(3):
            x_new = self.x_data[i][-1] + 1
            y_new = self.FPS

            self.y_data[i] = np.roll(self.y_data[i], -1)
            self.y_data[i][-1] = y_new

            self.x_data[i] = np.roll(self.x_data[i], -1)
            self.x_data[i][-1] = x_new

            self.curves[i].setData(self.x_data[i], self.y_data[i])

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RealTimePlotApp()
    window.show()
    sys.exit(app.exec_())