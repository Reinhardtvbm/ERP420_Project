#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

#include <QTimer>
#include <QUdpSocket>
#include <iostream>
#include <chrono>
#include "UDPServer.h"
#include <Eigen/Dense>

using namespace std::chrono;

using Eigen::MatrixXd;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // socket for receiving data
    UDPServer udp = UDPServer();

    // the refresh period in milliseconds for the plot
    const uint32_t refresh_period = 1000 / REFRESH_RATE;

    // array containing the pointers to each plot
    QCustomPlot* plots[3] = { ui->customPlot1, ui->customPlot2, ui->customPlot3 };

    double maxes[3] = {0.1, 2.0, 4.2};

    // colors of each subplot
    QColor colours[3] = {QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255)};

    // timer for refreshing the plot
    QTimer* timer = new QTimer();

    // x and y data for the plot
    QVector<QVector<double>> x(3), y(3);

    // initialise x and y data
    x.resize(3);
    y.resize(3);

    for (int i = 0; i < 3; ++i) {
        x[i].resize(1000);
        y[i].resize(1000);

        for (int j = 0; j < 1000; ++j) {
            x[i][j] = j;
            y[i][j] = 0;
        }

        // create three subplots:
        for (int k = 0; k < 3; ++k) {
            plots[i]->addGraph();
            plots[i]->graph(k)->setPen(QPen(colours[k]));
            plots[i]->graph(k)->setData(x[i], y[i]);
        }

        // give the axes some labels:
        plots[i]->xAxis->setLabel("x");
        plots[i]->yAxis->setLabel("y");
        plots[i]->yAxis->setRange(-maxes[i], maxes[i]);

        plots[i]->replot();
    }

    // variables for FPS/refresh rate calculation
    auto curr_time = high_resolution_clock::now();
    double y_new = 0.0, x_new = 0.0;
    double readings[9] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    // connect the timer's timeout signal to a function which updates the plots
    QObject::connect(timer, &QTimer::timeout, [=]() mutable {
        // calculate frames per second
        auto new_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(new_time - curr_time).count();
        curr_time = new_time;

        double FPS = 1000000.0 / duration;

        // get the new sensor readings from the Arduino ESP
        udp.receive(readings, FPS);

        MatrixXd m(2,2);
        m(0,0) = 3;
        m(1,0) = 2.5;
        m(0,1) = -1;
        m(1,1) = m(1,0) + m(0,1);
        std::cout << m << std::endl;

        for (int i = 0; i < 3; ++i) {
            QCustomPlot* plot = plots[i];

            for (int j = 0; j < 3; ++j) {
                QCPGraph* graph = plot->graph(j);
                auto data = graph->data();

                // new data
                y_new = readings[i * 3 + j];

                // remove first data point
                double begin_key = data->begin()->key;
                data->remove(begin_key);

                // add new data point
                x_new = begin_key + 1000;
                graph->addData(x_new, y_new);

                // rescale plot to fit new data
                plot->xAxis->setRange(x_new, 1000, Qt::AlignRight);
            }

            // refresh the plot
            plot->replot();
        }



    });

    // have the timer fire every refresh period
    timer->start(refresh_period);
}

MainWindow::~MainWindow()
{
    delete ui;
}

