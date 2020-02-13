#include "LBJPlot.h"

#include "src/Components/LabjackChannel.h"
#include "qcustomplot.h"

LBJPlot::LBJPlot(QCustomPlot *plot) : plot(plot) {
    timer = new QTimer;

    datapoints = 30;
    counter = 0;

    for(int i = 0; i < datapoints; i++) {
        time_axis.push_back(i);
        standard_axis.push_back(0);
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    create_layout();
}

LBJPlot::~LBJPlot() {}


void LBJPlot::update() {
    PlotElement *plotElement;

    if(counter >= datapoints) {
        shift_into_vector(time_axis, counter);
        foreach (plotElement, plotElement_list)
            shift_into_vector(plotElement->get_axis(), plotElement->get_channel()->get_value());
    }
    else {
        foreach (plotElement, plotElement_list)
            plotElement->get_axis()[int(counter)] = plotElement->get_channel()->get_value();
    }

    int maximum = get_maximum();
    int minimum = get_minimum();

    plot->yAxis->setRange(minimum, maximum);
    plot->yAxis->setTickStep((maximum-minimum)/4);

    int element_counter = 0;
    foreach (plotElement, plotElement_list) {
        if(plotElement->is_plotted())
            plot->graph(element_counter)->setData(time_axis, plotElement->get_axis());
        else
            plot->graph(element_counter)->setData(time_axis, standard_axis);
        element_counter++;
    }

    plot->xAxis->setRange(*std::min_element(time_axis.begin(), time_axis.end()), *std::max_element(time_axis.begin(), time_axis.end()));

    plot->replot();

    counter++;
}

void LBJPlot::create_layout() {
    plot->xAxis->setLabel("t[s]");
    plot->xAxis->setRange(0, datapoints);
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(datapoints/3);

    plot->yAxis->setLabel("[V]");
    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setRange(0, 30);

    plot->replot();
}

void LBJPlot::add_channel(LabjackChannel * channel, QColor color) {
    plotElement_list.push_back(new PlotElement{channel, standard_axis, false});
    plot->addGraph();
    plot->graph(plotElement_list.size() - 1)->setPen(QPen(color));
}

void LBJPlot::shift_into_vector(QVector<double> &vector, double value) {
    for(QVector<double>::iterator it = vector.begin(); it != vector.end()-1; it++) {
        (*it) = (*(it+1));
    }

    vector[vector.size()-1] = value;
}

int LBJPlot::maximum_function(int local_maximum, int absolut_maximum) {
    int new_maximum = 1;

    while(new_maximum < absolut_maximum) {
        if(local_maximum < 0.9*new_maximum)
            return new_maximum;
        new_maximum *= 2;
    }

    return absolut_maximum;
}

int LBJPlot::get_maximum() {
    int maximum = 0;

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list) {
        int new_maximum = maximum_function(int(*std::max_element(plotElement->get_axis().begin(), plotElement->get_axis().end())), 1000);

        if(new_maximum > maximum)
            maximum = new_maximum;
    }

    return maximum;
}

int LBJPlot::get_minimum() {
    int minimum = 0;

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list) {
        int new_maximum = maximum_function(qFabs(int(*std::min_element(plotElement->get_axis().begin(), plotElement->get_axis().end()))), 1000);

        if(new_maximum > minimum)
            minimum = new_maximum;
    }

    return -minimum;

}
