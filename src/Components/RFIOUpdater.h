#ifndef RFIOUPDATER_H
#define RFIOUPDATER_H

class RFIOChannel;

class QProcess;

#include <QObject>

class RFIOUpdater : public QObject {
    Q_OBJECT

public:
    RFIOUpdater(QProcess * process, QVector<RFIOChannel *> channel_list);
    virtual ~RFIOUpdater();

public slots:
    void process();

signals:
    void finished();
    void error();
};

#endif // RFIOUPDATER_H
