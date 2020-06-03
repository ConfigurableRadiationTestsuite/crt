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

enum EthernetStatus {Connected=1, Disconnected=0, Waiting=-1};

class Ethernet : public Component {
Q_OBJECT

public:
    Ethernet(RunManager *runManager, const QString &config);
    Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout);
    virtual ~Ethernet() override;

    uint get_port() const {return port;}
    long get_timeout() const {return timeout;}

    void reset_timeout();

    void set_config() override;

    void update() override;

public slots:
    void start_logging();
    void stop_logging();

    void accept_connection();
    void accept_data();

signals:
    void files_changed(const QString &);
    void bytes_changed(const QString &);
    void status_changed(int);

private:
    int port;
    long timeout;
    QString data_folder;

    QElapsedTimer *timer;
    QTcpServer *server;
    QTcpSocket *socket;

    long bytes = 0;
    long files = 0;

    void init();

    QVector<QString> generate_header() override;
};

#endif // ETHERNET_H
