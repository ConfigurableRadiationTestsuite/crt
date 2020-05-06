#include "Labjack.h"

#include "LabjackChannel.h"
#include "LJM_Utilities.h"
#include "src/Manager/RunManager.h"

#include <LabJackM.h>
#include <QElapsedTimer>

Labjack::Labjack(RunManager * runManager, const QString &config)
    : Logger(runManager) {
    qDebug("Create Labjack from Config");

    load_config(config);
    assert(parse_config({"name" , "channel"}));

    this->element_name = get_value("name");
    set_fileName(element_name);

    uint channel = get_value("channel").toUInt();

    open_labjack();

    for(uint i = 0; i < channel; i++) {
        QString name = get_value("c" + QString::number(i) + "n");

        int p_chan = get_value("c" + QString::number(i) + "pc").toInt();
        int n_chan = get_value("c" + QString::number(i) + "nc").toInt();

        double boundary = get_value("c" + QString::number(i) + "b").toDouble();
        int gain = get_value("c" + QString::number(i) + "g").toInt();

        channel_list.push_back(new LabjackChannel(name, &handle, p_chan, n_chan, gain, boundary));
    }

    init();
    update();
}

Labjack::Labjack(RunManager * runManager, const QString &m_element_name, const QString &channel_name, const QString &pchannel, const QString &nchannel)
    : Logger(runManager, m_element_name) {
    qDebug("Create Labjack from Scratch");

    this->element_name = m_element_name;

    QVector<QString> m_name;
    QVector<int> m_pchannel;
    QVector<int> m_nchannel;

    get_channel_names(channel_name, m_name);
    get_channel_addresses(pchannel, m_pchannel);
    get_channel_addresses(nchannel, m_nchannel);

    assert(m_pchannel.size() == m_nchannel.size());
    assert(m_name.size() == m_pchannel.size());

    open_labjack();

    for(int i = 0; i < m_name.size(); i++)
        channel_list.push_back(new LabjackChannel(m_name[i], &handle, m_pchannel[i], m_nchannel[i], 1, 0));

    init();
    update();
}

Labjack::~Labjack() {
    qDebug("Destroy Labjack");
    LJM_Close(handle);

    delete sampleTimer;

    //Delete channel
}

void Labjack::set_config() {
    config_entry_list.clear();

    set_value("name", element_name);
    set_value("channel", QString::number(channel_list.size()));

    for(int i = 0; i < channel_list.size(); i++) {
        set_value("c" + QString::number(i) + "n", channel_list[i]->get_name());
        set_value("c" + QString::number(i) + "pc", QString::number(channel_list[i]->get_pchan()));
        set_value("c" + QString::number(i) + "nc", QString::number(channel_list[i]->get_nchan()));
        set_value("c" + QString::number(i) + "b", QString::number(channel_list[i]->get_boundary()));
        set_value("c" + QString::number(i) + "g", QString::number(channel_list[i]->get_gain()));
    }
}

void Labjack::open_labjack() {
    is_connected = LJM_Open(LJM_dtANY, LJM_ctANY, "LJM_idANY", &handle) == 0 ? true : false;
}

void Labjack::init() {
    is_maximum = false;

    LabjackChannel* channel;
    foreach (channel, channel_list) {
        address_list.push_back(channel->get_pchan_address());
        type_list.push_back(LJM_FLOAT32);
        value_list.push_back(0.0);
    }

    sampleTimer = new QElapsedTimer;

    logTimer = new QTimer;
    logTimer->start(1000);
    connect(logTimer, SIGNAL(timeout()), this, SLOT(update()));
}

void Labjack::update() {
    sampleTimer->restart();

    /* Gather data */
    read(address_list, type_list, value_list);

    LabjackChannel* channel;
    QVector<double>::iterator it = value_list.begin();
    foreach (channel, channel_list) {
        channel->update_value(it != value_list.end() ? *it++ : 0);
        channel->set_range();
    }

    /* Check and set the sample rate */
    maxSamplerate = 1000 / (sampleTimer->elapsed() + 1);

    if(maxSamplerate < samplerate) {
        samplerate = maxSamplerate;
        emit samplerate_changed(QString::number(samplerate));
    }

    if(is_maximum && samplerate != maxSamplerate) {
        samplerate = maxSamplerate;
        emit samplerate_changed(QString::number(samplerate));
    }

    //Log data
    if(logging)
        runManager->append_values_to_file(this, value_list);
}

void Labjack::set_maximum_samplerate(int is_maximum) {
    this->is_maximum = is_maximum > 0 ? true : false;
}

void Labjack::set_main_settling(const QString &text) {
    int index = text.toInt();
    assert(0 < index && index <= 50000);
    write(get_main_settling_address(), LJM_FLOAT32, index);
}

void Labjack::set_main_resolution(int index) {
    assert(0 < index && index <= 10);
    write(get_main_resolution_address(), LJM_UINT16, index);
}

void Labjack::set_samplerate(const QString &text) {
    samplerate = text.toInt() > maxSamplerate ? maxSamplerate : text.toInt();
    samplerate = samplerate > 0 ? samplerate : 1;
    logTimer->start(1000/samplerate);

    emit samplerate_changed(QString::number(samplerate));
}

int Labjack::read(const QVector<int> &address, const QVector<int> &TYPE, QVector<double> &value) {
    if(is_connected == false)
        return 0;

    value.reserve(address.size());

    int err;
    int errorAddress = INITIAL_ERR_ADDRESS;

    int *aAddresses = new int[address.size()];
    int *aTypes = new int[address.size()];
    double *aValues = new double[address.size()];

    for(int i = 0; i < address.size(); i++) {
        aAddresses[i] = address[i];
        aTypes[i] = TYPE[i];
        aValues[i] = 0.0;
    }

    err = LJM_eReadAddresses(handle, address.size(), aAddresses, aTypes, aValues, &errorAddress);
    ErrorCheckWithAddress(err, errorAddress, "LJM_eReadAddresses");

    for(int i = 0; i < address.size(); i++)
        value[i] = aValues[i];

    delete[] aAddresses;
    delete[] aTypes;
    delete[] aValues;

    return err;
}

int Labjack::write(int address, const int TYPE, double value) {
    if(is_connected == false)
        return 0;

    return LJM_eWriteAddress(handle, address, TYPE, value);
}

int Labjack::read(int address, const int TYPE, double &value) {
    if(is_connected == false)
        return 0;

    return LJM_eReadAddress(handle, address, TYPE, &value);
}

QVector<QString> Labjack::generate_header() {
    QVector<QString> header;
    LabjackChannel * channel;
    foreach (channel, channel_list)
        header.push_back(channel->get_name());

    return header;
}

/* Reconstruct individual channels from user input */
void Labjack::get_channel_addresses(const QString &input, QVector<int> &output) {
    int position = 0;

    do {
        QString number = input.mid(position,  input.indexOf(',', position));
        output.push_back(number.toInt());
    } while((position = input.indexOf(',', position) + 1) > 0);
}

void Labjack::get_channel_names(const QString &input, QVector<QString> &output) {
    int position = 0;

     do {
        QString name = input.mid(position,  input.indexOf(',', position));
        name.remove(' ');
        output.push_back(name);
    } while((position = input.indexOf(',', position) + 1) > 0);
}
