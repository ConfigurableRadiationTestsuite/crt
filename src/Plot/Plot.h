#ifndef PLOT_H
#define PLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Abstract plot to provide easy use of
 * specialized plots
 *
 */

class QElapsedTimer;

#include "qcustomplot.h"

#include <QWidget>

class Plot : public QWidget {
Q_OBJECT

public:
    Plot(QCustomPlot *plot, int datapoints, int seconds);
    virtual ~Plot();

public slots:
    virtual void update_plot() = 0;
    void update_time_axis();

protected:
    QElapsedTimer *realTime;

    QCustomPlot *plot;
    QVector<double> timeAxis;

    int datapoints, seconds;
    long counter = 0;

    virtual void create_layout() = 0;

    virtual int get_maximum(const QVector<double> &data, int maximum);
    virtual int get_minimum(const QVector<double> &data, int minimum);
    int maximum_function(int local_maximum, int absolute_maximum);
    int minimum_function(int local_minimum, int absolute_minimum);

    void shift_into_vector(QVector<double> &vector, double value);

    void recreate_time_axis();
    void recreate_axis(QVector<double> &vec);
};

#endif // PLOT_H
