#include "EthernetClient.h"

#include <QTcpSocket>

EthernetClient::EthernetClient(uint port, const QString &address) : port(port), address(address) {
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    socket->connectToHost(address, port);
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

    if(!socket->waitForReadyRead(1000))
        return false;

    buffer = socket->readAll();

    return true;
}

bool EthernetClient::write(const QString &message) {
    if(!connection_ok)
        return false;

    return socket->write(message.toLocal8Bit()) == message.size();
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
