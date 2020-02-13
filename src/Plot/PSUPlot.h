#ifndef PSUPLOT_H
#define PSUPLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and
 * current of a single channel of the powersupply
 *
 */

class PSUChannel;

class QCustomPlot;

#include <QWidget>

class PSUPlot : public QWidget {
Q_OBJECT

public:
    PSUPlot(QCustomPlot *plot, PSUChannel * channel);
    virtual ~PSUPlot();


private slots:
    void update();

private:
    QTimer *timer;

    QCustomPlot *plot;
    QVector<double> time_axis;
    QVector<double> voltage_axis;
    QVector<double> current_axis;

    PSUChannel * channel;

    int datapoints;
    long counter;

    void create_layout();

    int get_max_voltage();
    int get_max_current();
    int maximum_function(int local_maximum, int absolute_maximum);

    void shift_into_vector(QVector<double> &vector, double value);
};

#endif // PSUPLOT_H
