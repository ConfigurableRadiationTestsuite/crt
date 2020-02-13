#ifndef PSUPLOT_H
#define PSUPLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and
 * current of a single channel of the powersupply
 *
 */

class LabjackChannel;

class QCustomPlot;

#include <QWidget>

class PlotElement : public QWidget {
Q_OBJECT

public:
    PlotElement(LabjackChannel * channel, QVector<double> axis, bool plot) :
        channel(channel), axis(axis), plot(plot) {}
    virtual ~PlotElement() {}

    QVector<double> & get_axis() {return axis;}
    LabjackChannel * get_channel() const {return channel;}
    bool is_plotted() const {return plot;}

public slots:
    void set_plot_active(int is_plotted) {plot = is_plotted > 0 ? true : false;}

private:
    LabjackChannel * channel;
    QVector<double> axis;
    bool plot;
};

class LBJPlot : public QWidget {
Q_OBJECT

public:
    LBJPlot(QCustomPlot *plot);
    virtual ~LBJPlot();

    void add_channel(LabjackChannel * channel, QColor color);
    QVector<PlotElement *> get_plotelement_list() {return plotElement_list;}

private slots:
    void update();

private:
    QTimer *timer;

    QCustomPlot *plot;
    QVector<double> time_axis;
    QVector<double> standard_axis;
    QVector<PlotElement *> plotElement_list;

    int datapoints;
    long counter;

    void create_layout();

    int get_minimum();
    int get_maximum();
    int maximum_function(int local_maximum, int absolute_maximum);

    void shift_into_vector(QVector<double> &vector, double value);
};

#endif // PSUPLOT_H
