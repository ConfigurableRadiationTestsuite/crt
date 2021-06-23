#ifndef FFOXPLOT_H
#define FFOXPLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Plotting the fieldfox spectrum analyzer graph
 *
 */

class Fieldfox;

#include "Plot.h"

class FFOXPlot : public Plot
{
Q_OBJECT

public:
    FFOXPlot(QCustomPlot* m_plot, int m_datapoints=30, int m_seconds=30);
    virtual ~FFOXPlot() override {}

public slots:
    void update_plot() override;

    void update_data(const QVector<double>& data);

private:
    QVector<double> xAxis;
    QVector<double> data;

    QElapsedTimer* plot_timer;

    void create_layout() override;

    void recreate_axis(QVector<double>& vec) override;
};

#endif // FFOXPLOT_H
