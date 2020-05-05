#ifndef DIOW_H
#define DIOW_H

/*
 * Author: Mattis Jaksch
 *
 */

class DIOM;

class QHBoxLayout;

#include "SubWindow.h"

class DIOW : public SubWindow {
Q_OBJECT

public:
    DIOW(DIOM *diom, RunManager *m_runManager);
    virtual ~DIOW() override;

private:
    DIOM * diom;

    QHBoxLayout * mainHLayout;

    void create_layout() override;
};

#endif // DIOW_H
