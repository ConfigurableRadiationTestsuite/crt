#ifndef PROGW_H
#define PROGW_H

/*
 * Author: Mattis Jaksch
 *
 */

#include "SubWindow.h"

class PROGW : public SubWindow {
Q_OBJECT

public:
    PROGW(EventManager *m_evenManager);
    virtual ~PROGW() override;

private:
    void create_layout() override;

};

#endif // PROGW_H
