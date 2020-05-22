#include "RFIOUpdater.h"

#include "RFIOChannel.h"

#include <QProcess>
#include <QtMath>
#include <QRandomGenerator>

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

void RFIOUpdater::stop() {
    qDebug("Stopping");
}

void RFIOUpdater::update_device() {
    static int offset = 0;
    RFIOChannel * channel;

    //Read data from QProcess
#ifndef DUMMY_DATA
    QByteArray data = process->readAllStandardOutput();
#endif
#ifdef DUMMY_DATA
    QByteArray data = dummy_iq(12, channel_list->size());
#endif
    char * real = new char[BYTE_PER_SAMPLE];
    char * imag = new char[BYTE_PER_SAMPLE];

    //Is the buffer cleared after a read?


    //Check how much data there is

    /* Distribute it to the data to the channels */
    int i, chan;
    for(i = 0; i < data.size() / (BYTE_PER_CHANNEL * channel_list->size()) + offset; i++) {
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

    offset = i * BYTE_PER_CHANNEL * channel_list->size() - data.size();

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

QByteArray RFIOUpdater::dummy_iq(int period, int channel) {
    QByteArray data;
    qint8 high_byte;
    qint8 low_byte;
    qint16 result;

    for(int i = 0; i < period*6; i++) {

        for(int j = 0; j < channel; j++) {
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
