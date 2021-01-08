#include "OSCChannel.h"

#include "EthernetClient.h"

#ifdef DUMMY_DATA
#include <QRandomGenerator>
#include <QtMath>
#endif

OSCChannel::OSCChannel(uint number, EthernetClient * eth, enum vendor vd) {
    this->number = number;
    this->eth = eth;
    this->vd = vd;
}

void OSCChannel::set_enable(int enable) {
    this->enable = enable == 0 ? false : true;

    if(!update())
        this->enable = false;

    emit enable_changed(this->enable);
}
