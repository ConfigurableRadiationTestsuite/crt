#include "RFPlot.h"

#include "src/Components/RFIOChannel.h"
#include "qcustomplot.h"

RFPlot::RFPlot(QCustomPlot *m_plot, RFIOChannel * channel)
    : Plot(m_plot, 256, 30), channel(channel) {

    connect(channel, SIGNAL(finished()), this, SLOT(update_plot()));

    recreate_axis(i_axis);
    recreate_axis(q_axis);

    create_layout();
}

RFPlot::~RFPlot() {}

void RFPlot::update_plot() {
    i_axis = channel->get_i_plot_data();
    q_axis = channel->get_q_plot_data();

    modify_time_axis();

    int max_i = get_maximum(i_axis, int(qPow(2, 16)));
    int max_q = get_maximum(q_axis, int(qPow(2, 16)));
    int maximum = max_i > max_q ? max_i : max_q;

    plot->yAxis->setRange(0, maximum);
    plot->yAxis->setTickStep(maximum/4);

    plot->graph(0)->setData(time_axis, i_axis);
    plot->graph(1)->setData(time_axis, q_axis);
    plot->xAxis->setRange(*std::min_element(time_axis.begin(), time_axis.end()), *std::max_element(time_axis.begin(), time_axis.end()));

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
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(datapoints/4);

    plot->yAxis->setLabel("I / Q");
    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setRange(-128, 128);

    plot->replot();
}

void RFPlot::modify_time_axis() {
    //Check if time axis is big enough

    time_axis.clear();
    time_axis.reserve(i_axis.size());

    for(int i = 0; i < i_axis.size(); i++)
        time_axis.push_back(i);

    //Else push back oder pop
}
