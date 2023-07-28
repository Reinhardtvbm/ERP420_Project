#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <iostream>
#include <chrono>
using namespace std::chrono;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // timer for refreshing the plot
    QTimer* timer = new QTimer();

    // x and y data for the plot
    QVector<double> x(1000), y(1000);

    for (int i=0; i<1000; ++i)
    {
        x[i] = i;
        y[i] = 0;
    }

    // create graph and assign data to it:
    ui->customPlot1->addGraph();
    ui->customPlot1->graph(0)->setData(x, y);

    // give the axes some labels:
    ui->customPlot1->xAxis->setLabel("x");
    ui->customPlot1->yAxis->setLabel("y");
    ui->customPlot1->replot();

    // variables for FPS/refresh rate calculation
    auto curr_time = high_resolution_clock::now();
    double FPS = 0.0;

    int i = 1000;

    // connect the timer's timeout signal to a function which updates the plots
    QObject::connect(timer, &QTimer::timeout, [=]() mutable {
        QCustomPlot* plot = ui->customPlot1;
        QCPGraph* graph1 = plot->graph(0);

        // calculate frames per second
        auto old_time = curr_time;
        curr_time = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(curr_time - old_time).count();

        double new_FPS = 1000000.0 / duration;
        FPS = 0.999*FPS + 0.001*new_FPS; // exponential filter for displaying FPS

        std::cout << FPS << " FPS\n";

        // remove first data point
        graph1->data()->remove(graph1->data()->begin()->key);

        // add new data point
        double x_val = i++;
        graph1->addData(x_val, FPS);

        // rescale plot to fit new data
        graph1->rescaleValueAxis();
        plot->xAxis->setRange(x_val, 1000, Qt::AlignRight);

        // refresh the plot
        plot->replot();
    });

    // start(0) means call QTimer::timeout signal (above) as fast as passible
    timer->start(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

