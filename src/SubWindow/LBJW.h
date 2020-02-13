#ifndef LBJW_H
#define LBJW_H

/*
 * Author: Mattis Jaksch
 *
 * Labjack window to visualize and control
 * one device from labjack
 *
 */

#include "SubWindow.h"

class Labjack;
class LBJPlot;

class QVBoxLayout;

struct ChannelColor {
    QString name;
    QColor color;
};

class LBJW : public SubWindow {
Q_OBJECT

public:
    LBJW(Labjack *lbj, EventManager *m_eventManager);
    virtual ~LBJW() override;

private:
    Labjack * lbj;
    LBJPlot * lbjplot;

    QVBoxLayout *subVerticalLayout;

    QVector<ChannelColor> color_list = {{"blue", Qt::blue}, {"red", Qt::red}, {"green", Qt::green}};

    void create_layout() override;
};

#endif // LBJW_H
