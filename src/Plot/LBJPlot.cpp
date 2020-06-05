#include "LBJPlot.h"

#include "src/Components/LabjackChannel.h"

LBJPlot::LBJPlot(QCustomPlot *m_plot, int m_datapoints, int m_seconds)
    : Plot(m_plot, m_datapoints, m_seconds) {

    recreate_axis(standard_axis);

    plot_timer = new QElapsedTimer;
    plot_timer->start();

    create_layout();
}

LBJPlot::~LBJPlot() {}

void LBJPlot::update_plot() {
    /* Return if plot is inactive */
    if(!plot_active || plot_timer->elapsed() < 10)
        return ;

    plot_timer->restart();

    PlotElement *plotElement;

    /* Fill either vector up or shift the whole vector to add one datapoint */
    if(counter >= datapoints) {
        foreach (plotElement, plotElement_list)
            shift_into_vector(plotElement->get_axis(), plotElement->get_channel()->get_value());
    }
    else {
        foreach (plotElement, plotElement_list)
            plotElement->get_axis()[int(counter)] = plotElement->get_channel()->get_value();
    }

    update_time_axis();

    /* Set y-axis */
    int maximum = get_total_maximum();
    int minimum = get_total_minimum();

    plot->yAxis->setRange(minimum, maximum);
    plot->yAxis->setTickStep((maximum-minimum)/4);

    /* Plot active elements */
    int element_counter = 0;
    plot_active = false;
    foreach (plotElement, plotElement_list) {
        if(plotElement->is_plotted()) {
            plot->graph(element_counter)->setData(timeAxis, plotElement->get_axis());
            plot_active = true;
        }
        else
            plot->graph(element_counter)->setData(timeAxis, standard_axis);
        element_counter++;
    }

    plot->xAxis->setRange(*std::min_element(timeAxis.begin(), timeAxis.end()), *std::max_element(timeAxis.begin(), timeAxis.end()));

    plot->replot();
    counter++;
}

void LBJPlot::create_layout() {
    plot->xAxis->setLabel("[s]");
    plot->xAxis->setRange(0, datapoints);
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(datapoints/10);

    plot->yAxis->setLabel("[ ]");
    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setRange(-10, 10);

    plot->replot();
}

void LBJPlot::add_channel(LabjackChannel * channel, QColor color) {
    plotElement_list.push_back(new PlotElement{channel, standard_axis, false});
    plot->addGraph();
    plot->graph(plotElement_list.size() - 1)->setPen(QPen(color));

    connect(plotElement_list.last(), SIGNAL(plot_active(bool)), this, SLOT(set_plot_active(bool)));
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

    counter = 0;
}

int LBJPlot::get_total_maximum() {
    int maximum = 0;

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list) {
        int new_maximum = get_maximum(plotElement->get_axis(), int(qPow(2, LBJ_ADC_BITS)));

        if(new_maximum > maximum)
            maximum = new_maximum;
    }

    return maximum;
}

int LBJPlot::get_total_minimum() {
    int minimum = 0;

    PlotElement *plotElement;
    foreach (plotElement, plotElement_list) {
        int new_minimum = get_minimum(plotElement->get_axis(), int(qPow(2, LBJ_ADC_BITS)));

        if(new_minimum < minimum)
            minimum = new_minimum;
    }

    return minimum;
}

void LBJPlot::set_plot_active(bool active) {
    if(!plot_active)
        recreate_time_axis();

    plot_active = active;
}
