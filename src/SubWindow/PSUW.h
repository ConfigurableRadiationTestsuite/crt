#ifndef PSUW_H
#define PSUW_H

/*
 * Author: Mattis Jaksch
 *
 * Powersupply window to visualize and control
 * one supply with certain channels.
 *
 */

class PSU;
class PSUPlot;

class QHBoxLayout;

#include "SubWindow.h"

class PSUW : public SubWindow {
Q_OBJECT

public:
    PSUW(PSU *psu, EventManager *m_eventManager);
    virtual ~PSUW() override;

private slots:
    //void set_button_on();
    //void set_button_off();
    //void set_button_disconnected();

private:
    PSU * psu;

    QHBoxLayout * mainHLayout;

    void create_layout() override;
};

#endif // PSUW_H
