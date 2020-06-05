#ifndef ETHERNETCLIENT_H
#define ETHERNETCLIENT_H

/*
 * Author: Mattis Jaksch
 *
 * Ethernet Transceiver
 * Creates a socket and connects to a server
 * Currently adapted to communicate via SCPI
 *
 */

class QTcpSocket;

#include <QObject>

class EthernetClient : public QObject {
Q_OBJECT

public:
    EthernetClient(uint port, const QString &address);
	virtual ~EthernetClient();

    bool write(const QString &message);
    bool query(const QString &message, QString &buffer);

    bool is_connected() const {return connection_ok;}

signals:
    void connection_status(bool);

private slots:
    void connected();
    void disconnected();

private:
    uint port;
    QString address;

    bool connection_ok = false;
    QTcpSocket *socket;

    bool read(QString &buffer);
};

#endif // ETHERNETCLIENT_H
