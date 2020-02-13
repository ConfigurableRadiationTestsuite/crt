#include "Plot.h"

#include "qcustomplot.h"

#include <QtMath>

Plot::Plot(QCustomPlot *plot) : plot(plot) {
    timer = new QTimer;

    datapoints = 30;
    counter = 0;

    for(int i = 0; i < datapoints; i++)
        time_axis.push_back(i);

    connect(timer, SIGNAL(timeout()), this, SLOT(update_plot()));
    timer->start(1000);

    create_layout();
}

Plot::~Plot() {}

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
