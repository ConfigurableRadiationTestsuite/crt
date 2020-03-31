#include "Indicator.h"

Indicator::Indicator(QIcon on, QIcon off, QIcon event){
    this->on = new QIcon(on);
    this->off = new QIcon(off);
    this->event = new QIcon(event);

    this->setIconSize(QSize(40, 20));

    setIcon(off);

    connect(this, SIGNAL(stateChanged(int)), this, SLOT(set_individual_icon(int)));
}

Indicator::~Indicator() {
    delete on;
    delete off;
    delete event;
}

void Indicator::set_individual_icon(int ic) {
    if(ic < 0)
        setIcon(*event);

    if(ic == 0)
        setIcon(*off);

    if(ic > 0)
        setIcon(*on);
}

void Indicator::set_event_icon() {
    setIcon(*event);
}
