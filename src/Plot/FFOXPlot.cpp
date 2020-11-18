#include "FFOXPlot.h"

#include "src/Components/Fieldfox.h"

FFOXPlot::FFOXPlot(QCustomPlot *m_plot, int m_datapoints, int m_seconds)
    : Plot(m_plot, m_datapoints, m_seconds) {
    create_layout();

    recreate_axis(xAxis);
}

FFOXPlot::~FFOXPlot() {}

void FFOXPlot::update_plot() {
    /* Set y-axis */
    //int minimum = get_minimum(data);
    //plot->yAxis->setRange(minimum, 0);

    //Set data
    plot->graph(0)->setData(xAxis, data);

    plot->replot();
}

void FFOXPlot::update_data(const QVector<double> &data) {
    this->data = data;

    if(this->data.size() != xAxis.size())
        recreate_axis(xAxis);

    update_plot();
}

void FFOXPlot::create_layout() {
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));

    plot->xAxis->setLabel("[ ]");
    plot->xAxis->setRange(0, datapoints);

    plot->yAxis->setLabel("[dBm]");
    plot->yAxis->setRange(-120, 0);

    plot->replot();
}

void FFOXPlot::recreate_axis(QVector<double> &vec) {
    vec.clear();
    vec.resize(datapoints);

    for(int i = 0; i < datapoints; i++)
        vec[i] = i;
}
