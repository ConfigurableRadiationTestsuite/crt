#ifndef LBJW_H
#define LBJW_H

/*
 * Author: Mattis Jaksch
 *
 * RF window to visualize the IQ data
 * of a (AD-IIO) device connected via ethernet
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
