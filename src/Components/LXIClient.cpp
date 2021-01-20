#include "LXIClient.h"

#include <QTimer>
#include <lxi.h>

const int LXIClient::timeout = 2000;

LXIClient::LXIClient(uint port, const QString &address) : port(port), address(address) {
    lxi_init();
    device = lxi_connect(address.toStdString().c_str(), port, "inst0", timeout, VXI11);
}

LXIClient::~LXIClient() {
    lxi_disconnect(device);
}

bool LXIClient::read(QString &buffer, int size) {
    char response[size];
    bool ok = lxi_receive(device, response, size, timeout);

    if(ok)
        buffer = (QString(response)).leftRef((QString(response)).indexOf('\n')).toString();
    else
        buffer = "";

    return ok;
}

bool LXIClient::write(QString message) {
    return lxi_send(device, message.toStdString().c_str(), message.size(), timeout);
}

bool LXIClient::query(const QString &message, QString &buffer, int size) {
    if(!write(message))
        return false;

    if(!read(buffer, size))
        return false;

    return true;
}
