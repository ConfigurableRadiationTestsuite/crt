#ifndef FFOXW_H
#define FFOXW_H

/*
 * Author: Mattis Jaksch
 *
 */

class Fieldfox;

#include "SubWindow.h"


class FFOXW : public SubWindow {
Q_OBJECT

public:
    FFOXW(RunManager *m_runManager, Fieldfox *fox);
    virtual ~FFOXW() override;

private:
    Fieldfox *fox;

    void create_layout() override;
};

#endif // FFOXW_H
