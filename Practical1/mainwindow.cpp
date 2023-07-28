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

    QTimer* timer = new QTimer();

    QVector<double> x(1001), y(1001); // initialize with entries 0..100
    for (int i=0; i<1001; ++i)
    {
        x[i] = i/50.0 - 1; // x goes from -1 to 1
        y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot1->addGraph();
    ui->customPlot1->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot1->xAxis->setLabel("x");
    ui->customPlot1->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
//    ui->customPlot1->xAxis->setRange(-1, 1);
//    ui->customPlot1->yAxis->setRange(0, 1);
    ui->customPlot1->replot();

    auto curr_time = high_resolution_clock::now();
    double FPS = 0.0;

    int i = 101;

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

        // add new data to x-y values
        double x_val = i++/50.0 - 1;

        x.removeFirst();
        y.removeFirst();
        x.append(x_val);
        y.append(FPS);

        graph1->setData(x, y, true);

        // rescale plot to fit new data
        graph1->rescaleValueAxis();
        plot->xAxis->setRange(x_val, 20, Qt::AlignRight);

        // refresh the plot
        plot->replot();
    });

    timer->start(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

