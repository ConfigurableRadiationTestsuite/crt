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
class QVBoxLayout;

#include "SubWindow.h"

class RFW : public SubWindow {
Q_OBJECT

public:
    RFW(RunManager *m_runManager, RFIO *rfio);
    virtual ~RFW() override;

private:
    QVBoxLayout *mainVLayout;
    RFIO * rfio;

    void create_layout() override;

};

#endif // LBJW_H
