#include "Plot.h"

#include "qcustomplot.h"

#include <QElapsedTimer>
#include <QtMath>

Plot::Plot(QCustomPlot *plot, int datapoints, int seconds)
    : plot(plot), datapoints(datapoints), seconds(seconds) {

    timer = new QTimer;
    real_time = new QElapsedTimer;
    real_time->start();

    counter = 0;

    recreate_time_axis();

    connect(timer, SIGNAL(timeout()), this, SLOT(update_plot()));
    timer->start(1000);
}

Plot::~Plot() {}

void Plot::update_time_axis() {
    if(counter >= datapoints) {
        long last_timepoint = real_time->elapsed() + long(double(seconds * 1000) / double(datapoints));
        shift_into_vector(time_axis, double(last_timepoint) / 1000);
    }
}

void Plot::recreate_time_axis() {
    int last_timepoint = int(real_time->elapsed() / 1000);
    time_axis.clear();
    time_axis.reserve(datapoints);

    for(int i = 0; i < datapoints; i++)
        time_axis.push_back(last_timepoint + i * (double(seconds) / double(datapoints)));
}

void Plot::recreate_axis(QVector<double> &vec) {
    vec.clear();
    vec.reserve(datapoints);

    for(int i = 0; i < datapoints; i++)
        vec.push_back(0.0);
}

void Plot::shift_into_vector(QVector<double> &vector, double value) {
    for(QVector<double>::iterator it = vector.begin(); it != vector.end()-1; it++) {
        (*it) = (*(it+1));
    }

    vector[vector.size()-1] = value;
}

int Plot::maximum_function(int local_maximum, int absolut_maximum) {
    int new_maximum = 1;

    while(new_maximum < absolut_maximum) {
        if(local_maximum < 0.9*new_maximum)
            return new_maximum;
        new_maximum *= 2;
    }

    return absolut_maximum;
}

int Plot::get_maximum(const QVector<double> &data, int maximum) {
    return maximum_function(int(*std::max_element(data.begin(), data.end())), maximum);
}

int Plot::get_minimum(const QVector<double> &data, int minimum) {
    int local_minimum = int(*std::min_element(data.begin(), data.end()));
    return maximum_function(int(qFabs(local_minimum)), minimum) * (local_minimum < 0 ? -1 : 1);
}
