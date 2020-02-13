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

#include "src/Configuration/ConfigElement.h"

class Ethernet : public ConfigElement {

public:
    Ethernet(const QString &config);
    Ethernet(const QString &address, uint port, const QString &data_folder, long long timeout);
    virtual ~Ethernet() override;

    QString get_address() const {return address;}
    uint get_port() const {return port;}
    long long get_timeout() const {return timeout;}
    QString get_data_folder() const {return data_folder;}

    long long get_timeout_timer();
    void reset_timeout();

    void set_config() override;

private:
    QString address;
    uint port;

    QString data_folder;

    long long timeout;
    QElapsedTimer *timer;

    bool isReceiver;
};

#endif // ETHERNET_H
