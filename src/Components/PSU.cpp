#include "PSU.h"

#include "EthernetClient.h"

PSU::PSU(RunManager * runManager, const QString &config)
    : Component(runManager, config) {

    load_config(config);
    assert(parse_config({"name", "vendor", "master", "address", "channel"}));

    this->vd = check_vendor(get_value("vendor"));
    this->master_switch = get_value("master").toInt();
    this->address = get_value("address");

    init_ethernet(address);

    uint channel_max = get_value("channel").toUInt();

    for(uint i = 0; i < channel_max; i++) {
        assert(parse_config({"c" + QString::number(i) + "vs", "c" + QString::number(i) + "cs", "c" + QString::number(i) + "vm", "c" + QString::number(i) + "cm"}));

        double voltage_set = get_value("c" + QString::number(i) + "vs").toDouble();
        double current_set = get_value("c" + QString::number(i) + "cs").toDouble();
        double voltage_max = get_value("c" + QString::number(i) + "vm").toDouble();
        double current_max = get_value("c" + QString::number(i) + "cm").toDouble();

        channel_list.push_back(new PSUChannel{i, eth, vd, voltage_set, current_set, voltage_max, current_max});
    }

    update_settings();
}

PSU::PSU(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &vendor, uint channel_max, double voltage_max, double current_max)
    : Component(m_element_name, runManager), address(address) {

    this->elementName = m_element_name;
    this->vd = check_vendor(vendor);

    init_ethernet(address);

    if(vd == rohdeSchwarz) // || vendor)
        master_switch = true;

    for(uint i = 0; i < channel_max; i++)
        channel_list.push_back(new PSUChannel{i, eth, vd, 0, 0, voltage_max, current_max});

    update_settings();
}

PSU::~PSU() {
    delete eth;

    foreach(PSUChannel *channel, channel_list)
        delete channel;
}

void PSU::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("vendor", check_vendor(vd));
    set_value("master", QString::number(master_switch));
    set_value("address", address);
    set_value("channel", QString::number(channel_list.size()));

    for(int i = 0; i < channel_list.size(); i++) {
        set_value("c" + QString::number(i) + "vs", QString::number(channel_list[i]->get_voltage_set()));
        set_value("c" + QString::number(i) + "cs", QString::number(channel_list[i]->get_current_set()));
        set_value("c" + QString::number(i) + "vm", QString::number(channel_list[i]->get_voltage_max()));
        set_value("c" + QString::number(i) + "cm", QString::number(channel_list[i]->get_current_max()));
    }

    update_settings();
}

void PSU::update() {
    //Check if the network connection is ok
    if(check_network_connection()) {

        /* Gather data */
        QVector<double> values;
        values.reserve(channel_list.size()*2);
        PSUChannel * channel;
        foreach (channel, channel_list) {
            channel->meas_voltage();
            channel->meas_current();
            values.push_back(channel->get_voltage_meas());
            values.push_back(channel->get_current_meas());
        }

        if(logging)
            runManager->append_values_to_file(this, values);
    }

    emit data_available();
}

enum vendor PSU::check_vendor(const QString &vendor) {
    if(vendor.contains("rohde", Qt::CaseInsensitive) || vendor.contains("schwarz", Qt::CaseInsensitive))
        return rohdeSchwarz;

    if(vendor.contains("tti", Qt::CaseInsensitive))
        return tti;

    return none;
}

QString PSU::check_vendor(enum vendor vd) {
    if(vd == tti)
        return "tti";

    if(vd == rohdeSchwarz)
        return "rohdeschwarz";

    return "none";
}

void PSU::init_ethernet(const QString &address) {
    QString ip_address = address.mid(0, address.indexOf(":"));
    uint port = (address.mid(address.indexOf(":") + 1, address.size() - address.indexOf(":") - 1)).toUInt();

    eth = new EthernetClient(port, ip_address);
}

void PSU::set_master_enable(int master_enable) {
    if(!check_network_connection())
        return ;

    this->master_enable = master_enable == 0 ? false : true;

    if(vd == rohdeSchwarz)
        set_master_rohdeschwarz();

//    if(vd == vendor)
//        set_master_vendor();

    emit master_changed(this->master_enable);
}

void PSU::set_master_rohdeschwarz() {
    eth->write("OUTP:MAST " + QString(master_enable ? "ON" : "OFF"));
}

void PSU::switch_on() {
    foreach (PSUChannel * channel, channel_list)
        if(channel->get_trigger())
            channel->set_enable(true);

    if(has_master_switch() && master_trigger)
        set_master_enable(1);
}

void PSU::switch_off() {
    if(has_master_switch() && master_trigger)
        set_master_enable(0);

    foreach (PSUChannel * channel, channel_list)
        if(channel->get_trigger())
            channel->set_enable(false);
}

bool PSU::check_network_connection() {
#ifndef DUMMY_DATA
    if(!eth->is_connected()) {
        emit disconnected(true);
        return false;
    }
#endif

    emit disconnected(false);
    return true;
}

QStringList PSU::generate_header() {
    QStringList header;

    foreach (PSUChannel * channel, channel_list) {
        header.push_back("V" + QString::number(channel->get_number()));
        header.push_back("mA" + QString::number(channel->get_number()));
    }

    return header;
}
