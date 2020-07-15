#ifndef ETHERNET_H
#define ETHERNET_H

/*
 * Author: Mattis Jaksch
 *
 * Ethernet Transceiver
 * If address is omitted it works as a receiver
 * If address is given it works as transmitter
 *
 * TODO: Build Reception and Transmission
 *
 */

class QElapsedTimer;
class QTcpServer;
class QTcpSocket;

#include "Component.h"

#include <QElapsedTimer>

enum EthernetStatus {Connected=1, Disconnected=0, Waiting=-1};

class Ethernet : public Component {
Q_OBJECT

public:
    Ethernet(RunManager *runManager, const QString &config);
    Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout);

    uint get_port() const {return port;}
    long get_timeout() const {return timeout;}

    void reset_timeout();

    void set_config() override;

    void update() override {}

public slots:
    void start_logging() override;
    void stop_logging() override;

private slots:
    void accept_connection();
    void accept_data();

    void set_data_folder();

    void handle_disconnection();

signals:
    void files_changed(const QString &);
    void bytes_changed(const QString &);
    void status_changed(int);
    void connection_timed_out();

private:
    int port;
    long timeout;
    QString data_folder;

    QTimer *timeoutTimer;
    QTcpServer *server;
    QTcpSocket *socket;

    long bytes = 0;
    long files = 0;

    void init();

    QStringList generate_header() override;
};

inline void Ethernet::reset_timeout() {
    timeoutTimer->start(timeout);
}

inline QStringList Ethernet::generate_header() {
    return {"File", "Bytes"};
}
#endif // ETHERNET_H
