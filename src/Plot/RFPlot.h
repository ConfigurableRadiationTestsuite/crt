#ifndef RFPLOT_H
#define RFPLOT_H

/*
 * Author: Mattis Jaksch
 *
 * Custom plot to show the voltage and
 * current of a single channel of the powersupply
 *
 */

class QCustomPlot;

#include <QWidget>

class RFPlot : public QWidget {
Q_OBJECT

public:
    RFPlot(QCustomPlot *plot);
    virtual ~RFPlot();

private:
    QCustomPlot *plot;
};

#endif // RFPLOT_H
