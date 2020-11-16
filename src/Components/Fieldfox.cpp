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
    update_measure();
}

void Fieldfox::init() {
    QString ip_address = address.mid(0, address.indexOf(":"));
    uint port = (address.mid(address.indexOf(":") + 1, address.size() - address.indexOf(":") - 1)).toUInt();

    eth = new EthernetClient(port, ip_address);

    connect(eth, SIGNAL(connection_status(bool)), this, SLOT(update_settings(bool)));

    emit init_done();
}

bool Fieldfox::update_measure() {
    if(!settings_ok)
        return false;

    QString buffer;
    eth->query("TRAC1:DATA?", buffer);

    int pos = 0, i = 0;
    while(pos < buffer.size() && i < points) {
        if(buffer.indexOf(",", pos) <= 0)
            break;

        int value_size = buffer.indexOf(",", pos) - pos;
        double value = buffer.mid(pos, value_size).toDouble();
        data[i] = value;

        pos += value_size + 1;
        i++;
    }

    //Save to vector
    if(logging)
        runManager->append_values_to_file(this, data);

    return true;
}

void Fieldfox::set_start_freq(QString const &text) {
    start_freq = text.toULong();

    eth->write("SENS:FREQ:START " + QString::number(start_freq));
}

void Fieldfox::set_stop_freq(QString const &text) {
    stop_freq = text.toULong();

    eth->write("SENS:FREQ:STOP " + QString::number(stop_freq));
}

void Fieldfox::set_points(QString const &text) {
    points = text.toULong();

    eth->write("SENS:SWE:POIN " + QString::number(points));

    data.resize(points);
}

/*void Fieldfox::set_format() {
    eth->write("FORM ASCii,0");
}

void Fieldfox::set_trace(uint trace) {
    eth->write("CALC:PAR" + QString::number(trace) + ":SEL");
}*/

void Fieldfox::update_settings(bool ok) {
    if(!ok) {
        settings_ok = false;
        return ;
    }

    set_start_freq(QString::number(start_freq));
    set_stop_freq(QString::number(stop_freq));
    set_points(QString::number(points));

    //set_format();
    //set_trace(1);

    configure_timer(3000);

    settings_ok = true;
}

QStringList Fieldfox::generate_header() {
    QStringList frequency_list;

    ulong frequency = start_freq;
    while(frequency < stop_freq) {
        frequency_list.push_back(QString::number(frequency));
        frequency += (stop_freq - start_freq) / (points - 1);
    }

    return frequency_list;
}
