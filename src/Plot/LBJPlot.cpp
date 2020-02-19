#include "LBJPlot.h"

#include "src/Components/LabjackChannel.h"
#include "qcustomplot.h"

LBJPlot::LBJPlot(QCustomPlot *m_plot, int m_datapoints, int m_seconds)
    : Plot(m_plot, m_datapoints, m_seconds) {

    recreate_axis(standard_axis);

    create_layout();
}

LBJPlot::~LBJPlot() {}


void LBJPlot::update_plot() {
    if(!plot_active)
        return ;

    PlotElement *plotElement;

    if(counter >= datapoints) {
        foreach (plotElement, plotElement_list)
            shift_into_vector(plotElement->get_axis(), plotElement->get_channel()->get_value());
    }
    else {
        foreach (plotElement, plotElement_list)
            plotElement->get_axis()[int(counter)] = plotElement->get_channel()->get_value();
    }

    update_time_axis();

    int maximum = get_total_maximum();
    int minimum = get_total_minimum();

    plot->yAxis->setRange(minimum, maximum);
    plot->yAxis->setTickStep((maximum-minimum)/4);

    int element_counter = 0;
    plot_active = false;
    foreach (plotElement, plotElement_list) {
        if(plotElement->is_plotted()) {
            plot->graph(element_counter)->setData(time_axis, plotElement->get_axis());
            plot_active = true;
        }
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
    plot->yAxis->setRange(-10, 10);

    plot->replot();
}

void LBJPlot::add_channel(LabjackChannel * channel, QColor color) {
    plotElement_list.push_back(new PlotElement{channel, standard_axis, false});
    plot->addGraph();
    plot->graph(plotElement_list.size() - 1)->setPen(QPen(color));

    connect(plotElement_list[plotElement_list.size()-1], SIGNAL(plot_active(bool)), this, SLOT(set_plot_active(bool)));
}

void LBJPlot::set_datarate(const QString &datarate) {
    int drate = datarate.toInt();

    if(drate < 1) drate = 1;
    if(drate > 100) drate = 100;

    datapoints = drate*seconds;

    recreate_time_axis();
    recreate_axis(standard_axis);

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list)
        recreate_axis(plotElement->get_axis());

    timer->start(1000/drate);
    counter = 0;
}

int LBJPlot::get_total_maximum() {
    int maximum = 0;

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list) {
        int new_maximum = get_maximum(plotElement->get_axis(), int(qPow(2, ADC_BITS)));

        if(new_maximum > maximum)
            maximum = new_maximum;
        }

    return maximum;
}

int LBJPlot::get_total_minimum() {
    int minimum = 0;

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list) {
        int new_minimum = get_minimum(plotElement->get_axis(), int(qPow(2, ADC_BITS)));

        if(new_minimum < minimum)
            minimum = new_minimum;
    }

    return minimum;
}

void LBJPlot::set_plot_active(bool active) {
    plot_active = active;
}
