#include "Ethernet.h"

#include <QElapsedTimer>

Ethernet::Ethernet(RunManager *runManager, const QString &config)
    : Component(runManager, config) {
    load_config(config);
    assert(parse_config({"name", "port", "timeout"}));

    elementName = get_value("name");
    port = get_value("port").toUInt();
    timeout = get_value("timeout").toLong();

    timer = new QElapsedTimer;
    timer->start();
}

Ethernet::Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout)
    : Component(m_element_name, runManager), port(port), timeout(timeout) {
    timer = new QElapsedTimer;
    timer->start();
}

Ethernet::~Ethernet() {}

long Ethernet::get_timeout_timer() {
    if(timer->elapsed() >= timeout)
        return 0;

    return (timeout - timer->elapsed());
}

void Ethernet::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("port", QString::number(port));
    set_value("timeout", QString::number(timeout));
}

void Ethernet::reset_timeout() {
    timer->restart();
}
