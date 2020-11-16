#include "Fieldfox.h"

#include "EthernetClient.h"

#ifdef DUMMY_DATA
#include <QRandomGenerator>
#endif

Fieldfox::Fieldfox(RunManager * runManager, const QString &config)
    : Component(runManager, config) {

    load_config(config);
    assert(parse_config({"name" , "address", "start_freq", "stop_freq", "points"}));

    this->elementName = get_value("name");
    this->address = get_value("address");
    this->mode = get_value("mode");
    this->start_freq = get_value("start_freq").toUInt();
    this->stop_freq = get_value("stop_freq").toUInt();
    this->points = get_value("points").toUInt();
}

Fieldfox::Fieldfox(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &mode, ulong start_freq, ulong stop_freq, ulong points)
    : Component(m_element_name, runManager), address(address), mode(mode), start_freq(start_freq), stop_freq(stop_freq), points(points) {
}

Fieldfox::~Fieldfox() {}

void Fieldfox::set_config() {
    config_entry_list.clear();

}

void Fieldfox::update() {

}

void Fieldfox::init() {

}

QVector<int> Fieldfox::measure() {
    QString ip_address = address.mid(0, address.indexOf(":"));
    uint port = (address.mid(address.indexOf(":") + 1, address.size() - address.indexOf(":") - 1)).toUInt();

    eth = new EthernetClient(port, ip_address);

//    update_settings();

    emit init_done();
}

QStringList Fieldfox::generate_header() {
    //List all available frequencies
}
