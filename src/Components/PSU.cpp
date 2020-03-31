#include "PSU.h"

#include "EthernetClient.h"
#include "src/Manager/RunManager.h"

#include <QTimer>

PSU::PSU(RunManager * runManager, const QString &config)
    : runManager(runManager) {
    qDebug("Create PSU from Config");

    load_config(config);
    assert(parse_config({"name", "vendor", "master", "address", "channel", "max_voltage", "max_current"}));

    this->element_name = get_value("name");
    this->vd = check_vendor(get_value("vendor"));
    this->master_switch = get_value("master").toUInt();
    this->address = get_value("address");

    init_ethernet(address);

    double channel_max = get_value("channel").toUInt();

    for(uint i = 0; i < channel_max; i++) {
        assert(parse_config({"c" + QString::number(i) + "v", "c" + QString::number(i) + "c"}));

        double voltage_set = get_value("c" + QString::number(i) + "vs").toDouble();
        double current_set = get_value("c" + QString::number(i) + "cs").toDouble();
        double voltage_max = get_value("c" + QString::number(i) + "vm").toDouble();
        double current_max = get_value("c" + QString::number(i) + "cm").toDouble();

        channel_list.push_back(new PSUChannel{i, eth, vd, voltage_set, current_set, voltage_max, current_max});
    }

    init();
}

PSU::PSU(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &vendor, uint channel_max, double voltage_max, double current_max)
    : runManager(runManager), address(address) {
    qDebug("Create PSU from scratch");

    this->element_name = m_element_name;
    this->vd = check_vendor(vendor);

    init_ethernet(address);

    if(vd == rohdeSchwarz)
        master_switch = true;

    for(uint i = 0; i < channel_max; i++)
        channel_list.push_back(new PSUChannel{i, eth, vd, 0, 0, voltage_max, current_max});

    init();
}

PSU::~PSU() {
    qDebug("Destroy PSU");
    delete eth;
}

void PSU::set_config() {
    config_entry_list.clear();

    set_value("name", element_name);
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
}

void PSU::init() {
    update_settings();

    log_timer = new QTimer;
    log_timer->start(1000);
    connect(log_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void PSU::start_logging() {
    qDebug("Start Log: " + element_name.toLatin1());

    runManager->register_component(this, element_name);
    runManager->set_file_header(this, generate_header());
    is_logging = true;
}

void PSU::stop_logging() {
    qDebug("Stop Log: " + element_name.toLatin1());

    runManager->deregister_component(this);
    is_logging = false;
}

void PSU::update() {
    QVector<double> values;
    PSUChannel * channel;
    foreach (channel, channel_list) {
        channel->meas_voltage();
        channel->meas_current();
        values.push_back(channel->get_voltage_meas());
        values.push_back(channel->get_current_meas());
    }

    if(is_logging)
        runManager->append_values_to_file(this, values);

    if(!eth->connectionOk())
        emit disconnected(true);
    else
        emit disconnected(false);
}

QVector<QString> PSU::generate_header() {
    QVector<QString> header;
    PSUChannel * channel;
    foreach (channel, channel_list) {
        header.push_back("V" + QString::number(channel->get_number()));
        header.push_back("mA" + QString::number(channel->get_number()));
    }

    return header;
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

    eth = new EthernetClient(port, ip_address.toStdString());
}

void PSU::set_master_enable(int master_enable) {
    this->master_enable = master_enable == 0 ? false : true;

    emit master_changed(this->master_enable);

    if(vd == rohdeSchwarz)
        set_master_rohdeschwarz();
}

void PSU::set_master_trigger(int master_trigger) {
    this->master_trigger = master_trigger > 0 ? true : false;
}

void PSU::update_settings() {
    qDebug("Update Settings: PSU");

    PSUChannel * channel;
    foreach (channel, channel_list)
        channel->update();
}

void PSU::set_master_rohdeschwarz() {
    if(!eth->connectionOk()) {
        eth->retry();
        return ;
    }

    eth->write("OUTP:MAST " + std::string(master_enable ? "ON" : "OFF"));
}

void PSU::switch_on() {
    qDebug("Switch On: PSU");

    PSUChannel * channel;
    foreach (channel, channel_list)
        if(channel->get_trigger())
            channel->set_enable(true);

    if(has_master_switch() && master_trigger)
        set_master_enable(1);
}

void PSU::switch_off() {
    qDebug("Switch Off: PSU");

    if(has_master_switch() && master_trigger)
        set_master_enable(0);

    PSUChannel * channel;
    foreach (channel, channel_list)
        if(channel->get_trigger())
            channel->set_enable(false);
}
