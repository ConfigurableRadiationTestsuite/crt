#ifndef RFPLOT_H
#define RFPLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and
 * current of a single channel of the powersupply
 *
 */

class RFIOChannel;

#include "Plot.h"

class RFPlot : public Plot {
Q_OBJECT

public:
    RFPlot(QCustomPlot *m_plot, RFIOChannel * channel);
    virtual ~RFPlot() override;

public slots:
    void update_plot() override;
    void update_layout();

private:
    QVector<double> i_axis;
    QVector<double> q_axis;

    RFIOChannel * channel;

    QTimer *layoutUpdateTimer;

    void create_layout() override;
    void modify_time_axis();
};

#endif // RFPLOT_H
