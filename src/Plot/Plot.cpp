#include "Plot.h"

#include <QElapsedTimer>
#include <QtMath>

Plot::Plot(QCustomPlot *plot, int datapoints, int seconds)
    : plot(plot), datapoints(datapoints), seconds(seconds) {

    realTime = new QElapsedTimer;
    realTime->start();

    recreate_time_axis();
}

Plot::~Plot() {}

void Plot::update_time_axis() {
    //qDebug("Current time: " + (QString::number(realTime->elapsed() / 1000.0)).toLatin1());

    if(counter >= datapoints) {
        long lastTimepoint = realTime->elapsed() + long(double(seconds * 1000) / double(datapoints));
        shift_into_vector(timeAxis, double(lastTimepoint) / 1000);
        //qDebug("Shift Axis: " + (QString::number(timeAxis.last())).toLatin1());
    }
}

void Plot::recreate_time_axis() {
    //qDebug("Current time: " + (QString::number(realTime->elapsed() / 1000.0)).toLatin1());

    long lastTimepoint = realTime->elapsed();
    timeAxis.clear();
    timeAxis.reserve(datapoints);

    for(int i = 0; i < datapoints; i++) {
        timeAxis.push_back(lastTimepoint / 1000.0 + i * (double(seconds) / double(datapoints)));
        //qDebug("Time Axis: " + (QString::number(timeAxis[i])).toLatin1());
    }
    //qDebug("Current time: " + (QString::number(realTime->elapsed() / 1000.0)).toLatin1());
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
