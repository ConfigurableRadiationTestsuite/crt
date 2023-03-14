#ifndef ETHERNET_H
#define ETHERNET_H

/*
 * Author: Mattis Jaksch
 *
 * Ethernet Transceiver
 * If address is omitted it works as a receiver
 * If address is given it works as transmitter
 *
 */

class QElapsedTimer;
class QTcpServer;
class QTcpSocket;

#include "Component.h"

#include <QElapsedTimer>

class Ethernet : public Component {
Q_OBJECT

public:
    enum Status {Connected=1, Disconnected=0, Waiting=-1};

    Ethernet(RunManager *runManager, const QString &config);
    Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout);
    virtual ~Ethernet();

    uint get_port() const {return port;}
    long get_timeout() const {return timeout;}
    Status get_status() const {return status;}

    void set_config() override;

    void reset_timeout();

public slots:
    void init() override;
    void update() override {}

    void start_logging() override;
    void stop_logging() override;
    void set_status(Status status);

private slots:
    void accept_connection();
    void accept_data();

    void set_data_folder();

    void handle_disconnection();

signals:
    void address_changed(const QString &);
    void files_changed(const QString &);
    void bytes_changed(const QString &);
    void status_changed(int);
    void connection_timed_out();

private:
    int port;
    long timeout;
    QString data_folder;

    QTimer *timeoutTimer;
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;

    Status status;

    long bytes = 0;
    long files = 0;

    QStringList generate_header() override;
};

inline void Ethernet::reset_timeout() {
    timeoutTimer->start(timeout);
}

inline QStringList Ethernet::generate_header() {
    return {"File", "Bytes"};
}

inline void Ethernet::set_status(Status status) {
    qDebug("Ethernet set status %d", status);
    this->status = status;
    emit status_changed(status);
}

#endif // ETHERNET_H
