#include "RFIO.h"

#include "src/Manager/RunManager.h"
#include "RFIOChannel.h"

#include <QFile>
#include <QDataStream>
#include <QProcess>
#include <QTimer>

RFIO::RFIO(RunManager * runManager, const QString &config) : runManager(runManager) {
    load_config(config);

    //Do things

    init();
}

RFIO::RFIO(RunManager * runManager, const QString &address, int channel)
    : runManager(runManager), address(address){

    for(int i = 0; i < channel; i++)
        channel_list.push_back(new RFIOChannel(i, buffersize));

    init();
}

RFIO::~RFIO() {

}

void RFIO::set_config() {
    config_entry_list.clear();

    set_value("name", element_name);
}


void RFIO::init() {
    log_timer = new QTimer;
    //connect(log_timer, SIGNAL(timeout()), this, SLOT(update_device()));
}

void RFIO::start_logging() {
    runManager->register_component(this, element_name);

    connect_device();

    log_timer->start(100);
}

void RFIO::stop_logging() {
    log_timer->stop();

    runManager->deregister_component(this);

    disconnect_device();
}

bool RFIO::connect_device() {
    int port = 9010;//(QString(address.mid(address.indexOf(':')))).toInt();

    process = new QProcess(this);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(update_device()));

    process->start("/bin/ncat -l " + QString::number(port));

    process->waitForStarted();


    return true;
}

void RFIO::disconnect_device() {
    process->kill();
}

void RFIO::update_device() {
    static int offset = 0;
    RFIOChannel * channel;

    //Read data from QProcess
    QByteArray data = process->readAllStandardOutput();
    char * real = new char[BYTE_PER_SAMPLE];
    char * imag = new char[BYTE_PER_SAMPLE];

    //Is the buffer cleared after a read?

    //Check how much data there is

    //Distribute it to the channels
    int i, chan;
    for(i = 0; i < data.size() / (BYTE_PER_CHANNEL * channel_list.size()); i++) {
        chan = 0;
        foreach(channel, channel_list) {
            real[0] = data[i*BYTE_PER_CHANNEL*channel_list.size() + chan*BYTE_PER_SAMPLE + offset];
            real[1] = data[i*BYTE_PER_CHANNEL*channel_list.size() + chan*BYTE_PER_SAMPLE + 1 + offset];

            imag[0] = data[i*BYTE_PER_CHANNEL*channel_list.size() + chan*BYTE_PER_SAMPLE + 2 + offset];
            imag[1] = data[i*BYTE_PER_CHANNEL*channel_list.size() + chan*BYTE_PER_SAMPLE + 3 + offset];

            channel->append_values(create_2b_number(real[0], real[1]), create_2b_number(imag[0], imag[1]));
            chan++;
        }
    }

    offset = (i + 1) * BYTE_PER_CHANNEL * channel_list.size() - data.size();

    //AND SAVE IT!

    //Analyize it
    foreach (channel, channel_list) {
        channel->update_data();
        channel->set_sample_position(channel->get_sample_position()+i*BYTE_PER_CHANNEL);
    }
}

int RFIO::create_2b_number(char lsb, char msb) {
    int result = static_cast<int8_t>(msb);
    result = result << 8;
    result += static_cast<uint8_t>(lsb);

    return result;
}
