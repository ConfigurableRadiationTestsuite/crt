#include "IndicatorIcon.h"
/*
IndicatorIcon::IndicatorIcon(QIcon onIcon, QIcon offIcon, QIcon eventIcon){
    this->onIcon = new QIcon(onIcon);
    this->offIcon = new QIcon(offIcon);
    this->eventIcon = new QIcon(eventIcon);

    this->setIconSize(QSize(40, 20));
    this->set_event_icon()

    setIcon(*this->offIcon);
    lastIcon = this->offIcon;

    connect(this, SIGNAL(stateChanged(int)), this, SLOT(set_individual_icon(int)));
}

IndicatorIcon::~IndicatorIcon() {
    delete onIcon;
    delete offIcon;
    delete eventIcon;
}

void IndicatorIcon::set_individual_icon(int ic) {
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

void IndicatorIcon::setDisabled(bool disabled) {
    setCheckable(!disabled);
    set_event_icon(true);
}

void IndicatorIcon::set_event_icon(bool status) {
    if(status == true)
        setIcon(*eventIcon);

    if(status == false)
        setIcon(*lastIcon);
}
*/
