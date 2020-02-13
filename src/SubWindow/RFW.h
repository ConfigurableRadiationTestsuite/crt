#ifndef LBJW_H
#define LBJW_H

/*
 * Author: Mattis Jaksch
 *
 * Labjack window to visualize and control
 * one device from labjack
 *
 */

class RFIO;

class QHBoxLayout;

#include "SubWindow.h"

class RFW : public SubWindow {
Q_OBJECT

public:
    RFW(RFIO *rfio, EventManager *m_eventManager);
    virtual ~RFW() override;

private:
    RFIO * rfio;
    QHBoxLayout *subHorizontalLayout;

    void create_layout() override;

};

#endif // LBJW_H
