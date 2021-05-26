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
    this->datapoints = get_value("points").toUInt();
}

Fieldfox::Fieldfox(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &mode, ulong start_freq, ulong stop_freq, uint datapoints)
    : Component(m_element_name, runManager), address(address), mode(mode), start_freq(start_freq), stop_freq(stop_freq), datapoints(datapoints) {

    this->elementName = m_element_name;
}

Fieldfox::~Fieldfox() {
    delete  eth;
}

void Fieldfox::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("address", address);
    set_value("mode", mode);
    set_value("start_freq", QString::number(start_freq));
    set_value("stop_freq", QString::number(stop_freq));
    set_value("points", QString::number(datapoints));
}

void Fieldfox::update() {
#ifdef DUMMY_DATA
    create_dummy_data(data);
    emit data_available(data);

    return ;
#endif

    if(!settings_ok)
        return ;

    if(!eth->is_connected())
        return ;

    update_measurement();
}

void Fieldfox::init() {
    QString ip_address = address.mid(0, address.indexOf(":"));
    uint port = (address.mid(address.indexOf(":") + 1, address.size() - address.indexOf(":") - 1)).toUInt();

    eth = new EthernetClient(port, ip_address);

    connect(eth, SIGNAL(connection_status(bool)), this, SLOT(update_settings(bool)));

    emit init_done();
}

bool Fieldfox::update_measurement() {
    /* Pull data */
    QString buffer;
    eth->query("TRAC1:DATA?", buffer, 17*datapoints);

    //TODO: Last one is not found
    uint pos = 0, i = 0;
    while((int)pos < buffer.size() && i < datapoints) {
        if(buffer.indexOf(",", pos) <= 0) {
            //Get last datapoint
            data[i] = buffer.mid(pos).toDouble();

            break;
        }

        int value_size = buffer.indexOf(",", pos) - pos;
        data[i] = buffer.mid(pos, value_size).toDouble();

        pos += value_size + 1;
        i++;
    }

    //Save to vector
    if(logging)
        runManager->append_values_to_file(this, data);

    emit data_available(data);

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
    datapoints = text.toULong();

    eth->write("SENS:SWE:POIN " + QString::number(datapoints));

    data.resize(datapoints);
}

void Fieldfox::set_frequency_span(const QString &text) {
    ulong frequency_span = (QString(text)).remove(" ").toULong();
    eth->write("FREQ:SPAN " + QString::number(frequency_span));
}

void Fieldfox::set_frequency_resolution(const QString &text) {
    ulong frequency_resolution = (QString(text)).remove(" ").toULong();
    eth->write("BAND " + QString::number(frequency_resolution));
}

void Fieldfox::set_sweep_time(const QString &text) {
    double sweep_time = text.toDouble();
    eth->write("SWE:ACQ " + QString::number(sweep_time));
}

void Fieldfox::set_mode(const QString &text) {
    eth->write("INST:SEL \"" + text + "\"");
    QString buffer;
    eth->query("*OPC?", buffer);
}

void Fieldfox::update_settings(bool ok) {
    if(!ok) {
        settings_ok = false;
        return ;
    }

    set_mode("SA");

    set_start_freq(QString::number(start_freq));
    set_stop_freq(QString::number(stop_freq));
    set_points(QString::number(datapoints));

    set_frequency_resolution("100");
    set_sweep_time("1");

    configure_timer(10000);

    settings_ok = true;
}

#ifdef DUMMY_DATA
void Fieldfox::create_dummy_data(QVector<double> &data) {
    data.resize(datapoints);

    for(int i = 0; i < data.size(); i++)
        data[i] = double(QRandomGenerator::global()->bounded(-qint16(100), qint16(0)));
}
#endif

QStringList Fieldfox::generate_header() {
    QStringList frequency_list;

    ulong frequency = start_freq;
    while(frequency <= stop_freq) {
        frequency_list.push_back(QString::number(frequency));
        frequency += (stop_freq - start_freq) / (datapoints - 1);
    }

    return frequency_list;
}
