#ifndef PLOT_H
#define PLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and
 * current of a single channel of the powersupply
 *
 */

class QElapsedTimer;
class QCustomPlot;

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
    QTimer *timer;
    QElapsedTimer *real_time;

    QCustomPlot *plot;
    QVector<double> time_axis;

    int datapoints, seconds;
    long counter;

    virtual void create_layout() = 0;

    virtual int get_minimum(const QVector<double> &data, int minimum);
    virtual int get_maximum(const QVector<double> &data, int maximum);
    int maximum_function(int local_maximum, int absolute_maximum);

    void shift_into_vector(QVector<double> &vector, double value);

    void recreate_time_axis();
    void recreate_axis(QVector<double> &vec);
};

#endif // PLOT_H