#include "RFIOUpdater.h"

#include "RFIOChannel.h"

#include <QProcess>

RFIOUpdater::RFIOUpdater(int port, QProcess * process, QVector<RFIOChannel *> *channel_list)
    : port(port), process(process), channel_list(channel_list){
}

RFIOUpdater::~RFIOUpdater () {}

void RFIOUpdater::start_process() {
    //connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(update_device()));

#ifndef DUMMY_DATA
    process->start("/bin/ncat -l " + QString::number(port));
#endif

#ifdef DUMMY_DATA
    process->start("/bin/cat /dev/urandom");
#endif

    process->waitForStarted();

    while(process->state() == QProcess::Running) {
        if(process->waitForReadyRead())
            update_device();
    }
}

void RFIOUpdater::update_device() {
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
    for(i = 0; i < data.size() / (BYTE_PER_CHANNEL * channel_list->size()); i++) {
        chan = 0;
        foreach(channel, *channel_list) {
            real[0] = data[i*BYTE_PER_CHANNEL*channel_list->size() + chan*BYTE_PER_SAMPLE + offset];
            real[1] = data[i*BYTE_PER_CHANNEL*channel_list->size() + chan*BYTE_PER_SAMPLE + 1 + offset];

            imag[0] = data[i*BYTE_PER_CHANNEL*channel_list->size() + chan*BYTE_PER_SAMPLE + 2 + offset];
            imag[1] = data[i*BYTE_PER_CHANNEL*channel_list->size() + chan*BYTE_PER_SAMPLE + 3 + offset];

            channel->append_value(create_2b_number(real[0], real[1]), create_2b_number(imag[0], imag[1]));
            chan++;
        }
    }

    offset = (i + 1) * BYTE_PER_CHANNEL * channel_list->size() - data.size();

    //Analyize it
    foreach (channel, *channel_list) {
        channel->analyze_data();
        channel->set_sample_position(channel->get_sample_position()+i*BYTE_PER_CHANNEL);
    }

    delete[] real;
    delete[] imag;
}

int RFIOUpdater::create_2b_number(char lsb, char msb) {
    int result = static_cast<int8_t>(msb);
    result = result << 8;
    result += static_cast<uint8_t>(lsb);

    return result;
}
