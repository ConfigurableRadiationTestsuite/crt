#include "IndicatorIcon.h"

#include <QLabel>
#include <QIcon>
#include <QTimer>

IndicatorIcon::IndicatorIcon(const QString &name, QPixmap connected, QPixmap disconnected, QPixmap event)
    : name(name) {

    this->connected = connected.scaled(QSize(48, 24));
    this->disconnected = disconnected.scaled(QSize(48, 24));
    this->event = event.scaled(QSize(48, 24));

    setNewPixmap(this->disconnected);

    eventTimer = new QTimer;
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(reset_icon()));
}

IndicatorIcon::~IndicatorIcon() {}

void IndicatorIcon::set_connected(int status) {
    if(status == 0)
        setNewPixmap(disconnected);
    else
        setNewPixmap(connected);
}

void IndicatorIcon::reset_icon() {
    eventTimer->stop();
    setPixmap(last);
}

void IndicatorIcon::set_event() {
    setPixmap(event);
    eventTimer->start(500);
}
