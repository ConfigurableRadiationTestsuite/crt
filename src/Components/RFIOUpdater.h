#ifndef RFIOUPDATER_H
#define RFIOUPDATER_H

#define BYTE_PER_SAMPLE 2
#define BYTE_PER_CHANNEL 4

class RFIOChannel;

class QProcess;

#include <QObject>

class RFIOUpdater : public QObject {
    Q_OBJECT

public:
    RFIOUpdater(int port, QProcess * process, QVector<RFIOChannel *> *channel_list);
    virtual ~RFIOUpdater();

public slots:
    void start_process();
    void update_device();
    void stop();

signals:
    void finished();
    void disconnected();

private:
    QProcess * process;
    QVector<RFIOChannel *> *channel_list;
    int port;

    int create_2b_number(char lsb, char msb);
    QByteArray dummy_iq(int period, int channel);
};

#endif // RFIOUPDATER_H
