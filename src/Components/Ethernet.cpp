#include "Ethernet.h"

#include <QElapsedTimer>

Ethernet::Ethernet(RunManager *runManager, const QString &config)
    : Component(runManager, config) {
    load_config(config);
    assert(parse_config({"name", "address", "port", "data", "timeout"}));

    elementName = get_value("name");
    address = get_value("address");
    isReceiver = address.size() <= 1 ? true : false;
    port = get_value("port").toUInt();
    data_folder = get_value("data");
    timeout = get_value("timeout").toLongLong();

    timer = new QElapsedTimer;
    timer->start();
}

Ethernet::Ethernet(RunManager *runManager, const QString &m_element_name, const QString &address, uint port, const QString &data_folder, long long timeout)
    : Component(m_element_name, runManager), address(address), port(port), data_folder(data_folder), timeout(timeout){
    timer = new QElapsedTimer;
    timer->start();

    isReceiver = address.size() <= 1 ? true : false;
}

Ethernet::~Ethernet() {}

long long Ethernet::get_timeout_timer() {
    if(timer->elapsed() >= timeout)
        return 0;

    return (timeout - timer->elapsed());
}

void Ethernet::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("address", address);
    set_value("port", QString::number(port));
    set_value("data", data_folder);
    set_value("timeout", QString::number(timeout));
}

void Ethernet::reset_timeout() {
    timer->restart();
}
