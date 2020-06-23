#include "EthernetClient.h"

#include <QTcpSocket>
#include <QTimer>

EthernetClient::EthernetClient(uint port, const QString &address) : port(port), address(address) {
    socket = new QTcpSocket(this);
    reconnection_timer = new QTimer(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    //connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(reconnection_timer, SIGNAL(timeout()), this, SLOT(disconnected()));

    socket->connectToHost(address, port);
    reconnection_timer->start(timeout);
}

EthernetClient::~EthernetClient() {
    socket->close();
}

void EthernetClient::connected() {
    connection_ok = true;
    emit connection_status(true);
}

void EthernetClient::disconnected() {
    connection_ok = false;
    emit connection_status(false);

    /* Attempt to reconnect */
    socket->reset();
    socket->connectToHost(address, port);
}

bool EthernetClient::read(QString &buffer) {
    if(!connection_ok)
        return false;

    if(!socket->waitForReadyRead(1000)) {
        disconnected();
        return false;
    }

    buffer = socket->readAll();

    if(buffer.size() > 0)
        reconnection_timer->start(timeout);

    return true;
}

bool EthernetClient::write(QString message) {
    if(!connection_ok)
        return false;

    message += "\r\n";
    int message_size = socket->write(message.toLocal8Bit());

    if(message_size > 0 )
        reconnection_timer->start(timeout);

    return  message_size == message.size();
}

bool EthernetClient::query(const QString &message, QString &buffer) {
    if(!connection_ok)
        return false;

    if(!write(message))
        return false;

    if(!read(buffer))
        return false;

    return true;
}
