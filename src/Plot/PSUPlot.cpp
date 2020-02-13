#include "PSUPlot.h"

#include "src/Components/PSUChannel.h"
#include "qcustomplot.h"

PSUPlot::PSUPlot(QCustomPlot *plot, PSUChannel *channel) : plot(plot), channel(channel) {
    timer = new QTimer;

    datapoints = 30;
    counter = 0;

    for(int i = 0; i < datapoints; i++) {
        time_axis.push_back(i);
        voltage_axis.push_back(0);
        current_axis.push_back(0);
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    create_layout();
}

PSUPlot::~PSUPlot() {}


void PSUPlot::update() {
    if(counter >= datapoints) {
        shift_into_vector(time_axis, counter);
        shift_into_vector(voltage_axis, channel->get_voltage_meas());
        shift_into_vector(current_axis, channel->get_current_meas());
    }
    else {
        voltage_axis[int(counter)] = channel->get_voltage_meas();
        current_axis[int(counter)] = channel->get_current_meas();
    }

    int max_voltage = get_max_voltage();
    int max_current = get_max_current();

    plot->yAxis->setRange(0, max_voltage);
    plot->yAxis->setTickStep(max_voltage/4);
    plot->yAxis2->setRange(0,max_current);
    plot->yAxis2->setTickStep(max_current/4);

    plot->graph(0)->setData(time_axis, voltage_axis);
    plot->graph(1)->setData(time_axis, current_axis);
    plot->xAxis->setRange(*std::min_element(time_axis.begin(), time_axis.end()), *std::max_element(time_axis.begin(), time_axis.end()));

    plot->replot();

    counter++;
}

void PSUPlot::create_layout() {
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));

    //Assign current graph to the second yAxis
    plot->addGraph(plot->xAxis, plot->yAxis2);
    plot->graph(1)->setPen(QPen(Qt::red));

    plot->xAxis->setLabel("t[s]");
    plot->xAxis->setRange(0, datapoints);
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(datapoints/3);

    plot->yAxis->setLabel("[V]");
    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setRange(0, 30);

    plot->yAxis2->setLabel("[mA]");
    plot->yAxis2->setRange(0, 3000);
    plot->yAxis2->setAutoTickStep(false);
    plot->yAxis2->setVisible(true);

    plot->replot();
}

void PSUPlot::shift_into_vector(QVector<double> &vector, double value) {
    for(QVector<double>::iterator it = vector.begin(); it != vector.end()-1; it++) {
        (*it) = (*(it+1));
    }

    vector[vector.size()-1] = value;
}

int PSUPlot::maximum_function(int local_maximum, int absolut_maximum) {
    int new_maximum = 1;

    while(new_maximum < absolut_maximum) {
        if(local_maximum < 0.9*new_maximum)
            return new_maximum;
        new_maximum *= 10;
    }

    return absolut_maximum;
}

int PSUPlot::get_max_current() {
    return maximum_function(int(*std::max_element(current_axis.begin(), current_axis.end())), int(channel->get_current_max()));
}

int PSUPlot::get_max_voltage() {
    return maximum_function(int(*std::max_element(voltage_axis.begin(), voltage_axis.end())), int(channel->get_voltage_max()));
}
