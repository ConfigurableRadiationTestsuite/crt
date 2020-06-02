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

#include "Component.h"

class Ethernet : public Component {
Q_OBJECT

public:
    Ethernet(RunManager *runManager, const QString &config);
    Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout);
    virtual ~Ethernet() override;

    uint get_port() const {return port;}
    long get_timeout() const {return timeout;}

    long get_timeout_timer();
    void reset_timeout();

    void set_config() override;

    void update() override;

public:
    void start_logging();
    void stop_logging();

private:
    uint port;

    QString data_folder;

    long timeout;
    QElapsedTimer *timer;

    QVector<QString> generate_header() override;
};

#endif // ETHERNET_H
