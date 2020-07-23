#ifndef ETHW_H
#define ETHW_H

/*
 * Author: Mattis Jaksch
 *
 * Shows ethernet packages being send / received
 * and the number of transmitted files / bytes
 *
 */

class Ethernet;

#include "SubWindow.h"

class ETHW : public SubWindow {
Q_OBJECT

public:
    ETHW(RunManager *m_runManager, Ethernet *ethernet);
    virtual ~ETHW() override;

private:
    Ethernet * ethernet;

    void create_layout() override;
};

#endif // PSUW_H
