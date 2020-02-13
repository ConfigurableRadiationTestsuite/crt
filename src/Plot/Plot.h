#ifndef PLOT_H
#define PLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and
 * current of a single channel of the powersupply
 *
 */

class QCustomPlot;

#include <QWidget>

class Plot : public QWidget {
Q_OBJECT

public:
    Plot(QCustomPlot *plot);
    virtual ~Plot();

private slots:
    virtual void update_plot() = 0;

protected:
    QTimer *timer;

    QCustomPlot *plot;
    QVector<double> time_axis;

    int datapoints;
    long counter;

    virtual void create_layout() = 0;

    virtual int get_minimum(const QVector<double> &data, int minimum);
    virtual int get_maximum(const QVector<double> &data, int maximum);
    int maximum_function(int local_maximum, int absolute_maximum);

    void shift_into_vector(QVector<double> &vector, double value);
};

#endif // PLOT_H
