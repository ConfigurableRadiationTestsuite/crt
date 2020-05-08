#include "RFPlot.h"

#include "src/Components/RFIOChannel.h"

RFPlot::RFPlot(QCustomPlot *m_plot, RFIOChannel * channel)
    : Plot(m_plot, 256, 30), channel(channel) {

    recreate_axis(i_axis);
    recreate_axis(q_axis);

    layoutUpdateTimer = new QTimer;
    connect(layoutUpdateTimer, SIGNAL(timeout()), this, SLOT(update_layout()));
    layoutUpdateTimer->start(1000);

    create_layout();
}

RFPlot::~RFPlot() {
    delete layoutUpdateTimer;
}

void RFPlot::update_plot() {
    i_axis = channel->get_i_plot_data();
    q_axis = channel->get_q_plot_data();

    plot->graph(0)->setData(timeAxis, i_axis);
    plot->graph(1)->setData(timeAxis, q_axis);

    plot->replot();
}

void RFPlot::update_layout() {
    modify_time_axis();

    int max_i = get_maximum(i_axis, int(qPow(2, 16)));
    int max_q = get_maximum(q_axis, int(qPow(2, 16)));
    int maximum = max_i > max_q ? max_i : max_q;

    int min_i = get_minimum(i_axis, int(qPow(2, 16)));
    int min_q = get_minimum(q_axis, int(qPow(2, 16)));
    int minimum = min_i > min_q ? min_i : min_q;

    plot->yAxis->setRange(minimum, maximum);
    plot->yAxis->setTickStep((maximum-minimum)/8);

    plot->xAxis->setRange(*std::min_element(timeAxis.begin(), timeAxis.end()), *std::max_element(timeAxis.begin(), timeAxis.end()));
    plot->xAxis->setRange(0, timeAxis.size());

    plot->replot();

    counter++;
}

void RFPlot::create_layout() {
    plot->addGraph();
    //Assign I-Data
    plot->graph(0)->setPen(QPen(Qt::blue));

    //Assign Q-Data
    plot->addGraph();
    plot->graph(1)->setPen(QPen(Qt::red));

    plot->xAxis->setLabel("Sample");
    plot->xAxis->setRange(0, datapoints);
    plot->xAxis->setAutoTickStep(true);
    plot->xAxis->setTickStep(datapoints/4);

    plot->yAxis->setLabel("I / Q");
    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setRange(-128, 128);

    plot->replot();
}

void RFPlot::modify_time_axis() {
    //Check if time axis is big enough

    timeAxis.clear();
    timeAxis.reserve(i_axis.size());

    for(int i = 0; i < i_axis.size(); i++)
        timeAxis.push_back(i);
}
