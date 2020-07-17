#include "RFIO.h"

#include "src/Manager/RunManager.h"

#include <QProcess>
#include <QRandomGenerator>

RFIO::RFIO(RunManager * runManager, const QString &config)
    : Component(runManager, config) {

    load_config(config);
    assert(parse_config({"name", "address", "channel"}));

    this->elementName = get_value("name");
    this->address = get_value("address");
    this->channel = get_value("channel").toInt();

    for(int i = 0; i < channel; i++) {
        assert(parse_config({"c" + QString::number(i) + "m"}));

        channel_list.push_back(new RFIOChannel(i, get_value("c" + QString::number(i) + "m").toInt()));
        connect(channel_list[i], SIGNAL(error(QVector<int>, QVector<int>, int)), this, SLOT(handle_error(QVector<int>, QVector<int>, int)));
    }

    init();
}

RFIO::RFIO(RunManager * runManager, const QString &m_element_name, const QString &address, int channel)
    : Component(m_element_name, runManager), address(address), channel(channel) {

    this->elementName = m_element_name;

    for(int i = 0; i < channel; i++) {
        channel_list.push_back(new RFIOChannel(i));
        connect(channel_list[i], SIGNAL(error(QVector<IQSample>, int)), this, SLOT(handle_error(QVector<IQSample>, int)));
    }

    init();
}

RFIO::~RFIO() {
    foreach(RFIOChannel *channel, channel_list)
        delete channel;

    process->kill();
    delete process;
}

void RFIO::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("address", address);
    set_value("channel", QString::number(channel));

    for(int i = 0; i < channel; i++)
        set_value("c" + QString::number(i) + "m", QString::number(channel_list[i]->get_margin()));
}

void RFIO::init() {
    //Create reception process
    process = new QProcess;
#ifndef DUMMY_DATA
    process->start("/bin/ncat -l " + QString::number(port), {""});
#endif

#ifdef DUMMY_DATA
    process->start("/bin/cat", {"/dev/urandom"});
#endif

    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(update()));

    process->waitForStarted();
}

void RFIO::update() {
    static int offset = 0;
    RFIOChannel * channel;

    //Read data from QProcess
#ifndef DUMMY_DATA
    QByteArray data = process->readAllStandardOutput();
#endif
#ifdef DUMMY_DATA
    QByteArray data = dummy_iq(30, channel_list.size());
#endif

    /* Distribute it to the data to the channels */
    int i, chan;
    for(i = 0; i < data.size() / (BYTE_PER_CHANNEL * channel_list.size()); i++) {
        chan = 0;
        foreach(channel, channel_list) {
            channel->append_value(data.mid(i*BYTE_PER_CHANNEL*channel_list.size() + chan*BYTE_PER_SAMPLE + offset, 4));
            chan++;
        }
    }

    offset = i * BYTE_PER_CHANNEL * channel_list.size() - data.size();

    //Analyize it
    foreach (channel, channel_list) {
        channel->analyze_data();
        channel->set_sample_position(channel->get_sample_position()+i*BYTE_PER_CHANNEL);
    }
}

QByteArray RFIO::dummy_iq(int period, int channel) {
    QByteArray data;
    qint8 high_byte;
    qint8 low_byte;
    qint16 result;

    for(int i = 0; i < period*12; ++i) {

        for(int j = 0; j < channel; ++j) {
            /* IQ-Data */
            result = qSin(float(i)/float(period) * 2 * M_PI) * qPow(2, 10) + QRandomGenerator::global()->bounded(-qint16(qPow(2, 4)), qint16(qPow(2, 4)));
            high_byte = (result & 0b1111111100000000) >> 8;
            low_byte = result & 0b0000000011111111;

            data.push_back(low_byte);
            data.push_back(high_byte);

            result = qCos(float(i)/float(period) * 2 * M_PI) * qPow(2, 10) + QRandomGenerator::global()->bounded(-qint16(qPow(2, 4)), qint16(qPow(2, 4)));
            high_byte = (result & 0b1111111100000000) >> 8;
            low_byte = result & 0b0000000011111111;

            data.push_back(low_byte);
            data.push_back(high_byte);
        }
    }

    return data;
}

QStringList RFIO::generate_header() {
    QStringList header;
    header.push_back("Sample");

    RFIOChannel * channel;
    foreach (channel, channel_list) {
        header.push_back("I" + QString::number(channel->get_channel_number()));
        header.push_back("Q" + QString::number(channel->get_channel_number()));
    }

    return header;
}

void RFIO::handle_error(QVector<IQSample> data, int number) {
    /* Write the raw data to the runManager */
    if(!logging)
        return;

    if(is_single_shot) {
        process->kill();
    }

    for(int i = 0; i < data.size(); i++) {
        QVector<double> row;
        row.push_back(i);

        for(int j = 0; j < channel_list.size(); j++) {
            if(j == number) {
                row.push_back(data[i].get_i());
                row.push_back(data[i].get_q());
            }
            else {
                row.push_back(0);
                row.push_back(0);
            }
        }
        runManager->append_values_to_file(this, row);
    }
}

