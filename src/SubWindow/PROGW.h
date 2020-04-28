#ifndef PROGW_H
#define PROGW_H

/*
 * Author: Mattis Jaksch
 *
 */

class ProgrammStarter;

#include "SubWindow.h"

class PROGW : public SubWindow {
Q_OBJECT

public:
    PROGW(ProgrammStarter * programmStarter, EventManager *m_evenManager);
    virtual ~PROGW() override;

private:
    ProgrammStarter * programmStarter;

    void create_layout() override;

};

#endif // PROGW_H
