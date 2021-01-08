#ifndef RFPLOT_H
#define RFPLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and of an oscilloscope
 *
 */

class OSC;

#include "Plot.h"

class OSCPlot : public Plot {
Q_OBJECT

public:
    OSCPlot(QCustomPlot *m_plot, OSC *osc);
    virtual ~OSCPlot() override;

public slots:
    void update_plot() override;
    void update_layout();
    void update_data(const QVector<int> &i_axis, const QVector<int> &q_axis);

private:
    QVector<double> i_axis;
    QVector<double> q_axis;

    OSC *osc;

    QTimer *layoutUpdateTimer;
    QTimer *plotUpdateTimer;

    void create_layout() override;
    void modify_time_axis();
};

#endif // RFPLOT_H
