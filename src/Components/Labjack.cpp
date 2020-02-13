#include "Labjack.h"

#include "LabjackChannel.h"
#include "src/Manager/RunManager.h"
#include "LJM_Utilities.h"

#include <LabJackM.h>
#include <QTimer>
#include <QElapsedTimer>

Labjack::Labjack(RunManager * runManager, const QString &config) : runManager(runManager) {
    load_config(config);
    assert(parse_config({"name"}));

    element_name = ConfigElement::get_value("name");
    uint channel = ConfigElement::get_value("channel").toUInt();

    open_labjack();

    for(uint i = 0; i < channel; i++) {
        QString name = ConfigElement::get_value("c" + QString::number(i) + "n");

        int p_chan = ConfigElement::get_value("c" + QString::number(i) + "pc").toInt();
        int n_chan = ConfigElement::get_value("c" + QString::number(i) + "nc").toInt();

        double boundary = ConfigElement::get_value("c" + QString::number(i) + "b").toDouble();
        int gain = ConfigElement::get_value("c" + QString::number(i) + "g").toInt();

        channel_list.push_back(new LabjackChannel(name, &handle, p_chan, n_chan, gain, boundary));
    }

    init();
    update();
}

Labjack::Labjack(RunManager * runManager, const QString &name, const QString &type, const QString &channel_name, const QString &pchannel, const QString &nchannel) {
    //TODO Implement this
}

Labjack::~Labjack() {
    LJM_Close(handle);
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
    is_logging = false;

    LabjackChannel* channel;
    foreach (channel, channel_list) {
        address_list.push_back(channel->get_pchan_address());
        type_list.push_back(LJM_FLOAT32);
        value_list.push_back(0.0);
    }

    sampleTimer = new QElapsedTimer;

    timer = new QTimer;
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Labjack::start_logging() {
    runManager->register_component(this, element_name);
    runManager->set_file_header(this, generate_header());
    is_logging = true;
}

void Labjack::stop_logging() {
    runManager->deregister_component(this);
    is_logging = false;
}

void Labjack::start() {
    start_logging();
}

void Labjack::stop() {
    stop_logging();
}

void Labjack::update() {
    sampleTimer->restart();

    read(address_list, type_list, value_list);

    LabjackChannel* channel;
    QVector<double>::iterator it = value_list.begin();
    foreach (channel, channel_list) {
        channel->update_value(it != value_list.end() ? *it++ : 0);
        channel->set_range();
    }

    maxSamplerate = 1000 / (sampleTimer->elapsed() + 1);

    if(maxSamplerate < samplerate) {
        samplerate = maxSamplerate;
        emit samplerate_changed(QString::number(samplerate));
    }

    if(is_maximum && samplerate != maxSamplerate) {
        samplerate = maxSamplerate;
        emit samplerate_changed(QString::number(samplerate));
    }

    if(is_logging)
        runManager->append_values_to_file(this, value_list);
}

void Labjack::set_maximum_samplerate(int is_maximum) {
    this->is_maximum = is_maximum > 0 ? true : false;
}

void Labjack::set_main_settling(const QString &text) {
    int index = text.toInt();
    write(get_main_settling_address(), LJM_FLOAT32, index);
}

void Labjack::set_main_resolution(int index) {
    assert(0 <= index && index <= 10);
    write(get_main_resolution_address(), LJM_UINT16, index);
}

void Labjack::set_samplerate(const QString &text) {
    samplerate = text.toInt() > maxSamplerate ? maxSamplerate : text.toInt();
    samplerate = samplerate > 0 ? samplerate : 1;
    timer->start(1000/samplerate);

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
