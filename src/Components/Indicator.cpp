#include "Indicator.h"

Indicator::Indicator(QIcon onIcon, QIcon offIcon, QIcon eventIcon){
    this->onIcon = new QIcon(onIcon);
    this->offIcon = new QIcon(offIcon);
    this->eventIcon = new QIcon(eventIcon);

    this->setIconSize(QSize(40, 20));

    setIcon(*this->offIcon);
    lastIcon = this->offIcon;

    connect(this, SIGNAL(stateChanged(int)), this, SLOT(set_individual_icon(int)));
}

Indicator::~Indicator() {
    delete onIcon;
    delete offIcon;
    delete eventIcon;
}

void Indicator::set_individual_icon(int ic) {
    if(ic < 0)
        setIcon(*eventIcon);

    if(ic == 0) {
        setIcon(*offIcon);
        lastIcon = offIcon;
    }

    if(ic > 0) {
        setIcon(*onIcon);
        lastIcon = onIcon;
    }
}

void Indicator::setDisabled(bool disabled) {
    setCheckable(!disabled);
    set_event_icon(true);
}

void Indicator::set_event_icon(bool status) {
    if(status == true)
        setIcon(*eventIcon);

    if(status == false)
        setIcon(*lastIcon);
}
