#include "EthernetClient.h"

#include <QTcpSocket>
#include <QTimer>

EthernetClient::EthernetClient(uint port, const QString &address) : port(port), address(address) {
    socket = new QTcpSocket;
    reconnection_timer = new QTimer;

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    //connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(reconnection_timer, SIGNAL(timeout()), this, SLOT(disconnected()));

    socket->connectToHost(address, port);
    qDebug((QString::number(socket->state())).toLatin1());

    reconnection_timer->start(timeout);
}

EthernetClient::~EthernetClient() {
    socket->close();

    delete socket;
    delete reconnection_timer;
}

void EthernetClient::connected() {
    qDebug((QString::number(socket->state())).toLatin1());
    connection_ok = true;
    emit connection_status(true);
}

void EthernetClient::disconnected() {
    qDebug((QString::number(socket->state())).toLatin1());
    connection_ok = false;
    emit connection_status(false);

    /* Attempt to reconnect */
    socket->reset();
    socket->connectToHost(address, port);
}

bool EthernetClient::read(QString &buffer, int size) {
    if(!connection_ok)
        return false;

    static int reads = 0;

    if(!socket->waitForReadyRead(1000)) {
        disconnected();
        return false;
    }

    buffer = socket->readAll();

    //qDebug("Expected size: " + (QString::number(size)).toLatin1());
    int terminator = 0;
    while(terminator < 3 && buffer.size() < size) {
        if(socket->waitForReadyRead(10)) {
            buffer.append(socket->readAll());
            terminator = 0;
        }
        else
            terminator++;
    }
    //qDebug("Received size: " + (QString::number(buffer.size())).toLatin1());

    if(buffer.size() > 0)
        reconnection_timer->start(timeout);

    reads++;
    qDebug("Successfull read number: " + (QString::number(reads)).toLatin1());

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

bool EthernetClient::query(const QString &message, QString &buffer, int size) {
    if(!connection_ok)
        return false;

    if(!write(message))
        return false;

    if(!read(buffer, size))
        return false;

    return true;
}
