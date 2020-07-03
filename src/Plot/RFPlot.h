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
    void update_data(const QVector<int> &i_axis, const QVector<int> &q_axis);

private:
    QVector<double> i_axis;
    QVector<double> q_axis;

    RFIOChannel * channel;

    QTimer *layoutUpdateTimer;
    QTimer *plotUpdateTimer;

    void create_layout() override;
    void modify_time_axis();
};

inline void RFPlot::update_data(const QVector<int> &i_axis, const QVector<int> &q_axis) {
    this->i_axis = QVector<double>(i_axis.begin(), i_axis.end());
    this->q_axis = QVector<double>(q_axis.begin(), q_axis.end());
}

#endif // RFPLOT_H
