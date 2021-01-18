#ifndef LXICLIENT_H
#define LXICLIENT_H

/*
 * Author: Mattis Jaksch
 *
 * LXI Client for communication with weird power supplies
 *
 */

class QTcpSocket;
class QTimer;

#include <QObject>

class LXIClient : public QObject {
Q_OBJECT

public:
    LXIClient(uint port, const QString &address);
    virtual ~LXIClient();

    bool write(QString message);
    bool query(const QString &message, QString &buffer, int size=65536);

signals:
    void connection_status(bool);

private:
    uint port;
    QString address;
    int device;

    static const int timeout;

    bool read(QString &buffer, int size);
};

#endif // LXICLIENT_H
