#include "RFIO.h"

#include "src/Manager/RunManager.h"
#include "RFIOChannel.h"

#include <QFile>
#include <QDataStream>
#include <QTimer>

RFIO::RFIO(RunManager * runManager, const QString &config) : runManager(runManager) {
}

RFIO::RFIO(RunManager * runManager, const QString &address, int channel)
    : runManager(runManager), address(address){
    for(int i = 0; i < channel; i++)
        channel_list.push_back(new RFIOChannel(i, buffersize));
}

RFIO::~RFIO() {

}


void RFIO::init() {
    log_timer = new QTimer;
    connect(log_timer, SIGNAL(timeout()), this, SLOT(update_channel()));
}

void RFIO::start_logging() {
    connect_to_device();
    log_timer->start(100);
}

void RFIO::stop_logging() {
    runManager->deregister_component(this);
    log_timer->stop();
}

bool RFIO::connect_to_device() {
    runManager->register_component(this, element_name);

    int port = (QString(address.mid(address.indexOf(':')))).toInt();
    QString systemcall = "ncat -l " + QString::number(port) + " > " + runManager->get_file(this)->name + " &";
    system(systemcall.toUtf8());
}

void RFIO::update_channel() {
    RFIOChannel * channel;
    QDataStream fstream(runManager->get_file(this)->file);
    ssize_t filesize = fstream.skipRawData(-1);
    ssize_t sample_size = 0;

    foreach (channel, channel_list) {
        channel->clear_data();
        sample_size += channel->get_sample_position();
    }

    if(filesize < buffersize*BYTE_PER_CHANNEL*channel_list.size() + sample_size*BYTE_PER_CHANNEL)
        return ;

    char *real, *imag;
    real = new char[BYTE_PER_SAMPLE];
    imag = new char[BYTE_PER_SAMPLE];

    for(int i = 0; i < buffersize; i++) {

        foreach(channel, channel_list) {
            fstream.readRawData(real, BYTE_PER_SAMPLE);
            fstream.readRawData(imag, BYTE_PER_SAMPLE);

            channel->append_values(create_2b_number(real[0], real[1]), create_2b_number(imag[0], imag[1]));
        }
    }

    foreach (channel, channel_list) {
        channel->update_data();
        channel->set_sample_position(channel->get_sample_position()+buffersize);
    }
}

int RFIO::create_2b_number(char msb, char lsb) {
    int result = static_cast<int8_t>(msb);
    result = result << 8;
    result += static_cast<uint8_t>(lsb);

    return result;
}
