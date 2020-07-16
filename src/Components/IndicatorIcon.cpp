#include "IndicatorIcon.h"

#include <QLabel>
#include <QIcon>
#include <QTimer>

IndicatorIcon::IndicatorIcon(const QString &name, QPixmap connected, QPixmap disconnected,  QPixmap waiting, QPixmap event)
    : name(name) {

    this->connected = connected.scaled(QSize(48, 24));
    this->disconnected = disconnected.scaled(QSize(48, 24));
    this->waiting = waiting.scaled(QSize(48, 24));
    this->event = event.scaled(QSize(48, 24));

    setNewPixmap(this->waiting);

    eventTimer = new QTimer;
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(reset_icon()));
}

IndicatorIcon::~IndicatorIcon() {}

void IndicatorIcon::set_status(int status) {
    switch(status) {
        case -1:
            setNewPixmap(waiting);
            break;
        case 0:
            setNewPixmap(disconnected);
            break;
        case 1:
            setNewPixmap(connected);
            break;
    }
}

void IndicatorIcon::reset_icon() {
    eventTimer->stop();
    setPixmap(last);
}

void IndicatorIcon::set_event() {
    setPixmap(event);
    eventTimer->start(500);
}
