#ifndef LOGGER_H
#define LOGGER_H

#include "src/Manager/RunManager.h"

#include <QObject>
#include <QTimer>

class Logger : public QObject {
Q_OBJECT

public:
    Logger(RunManager *runManager);
    Logger(RunManager *runManager, const QString &fileName);
    virtual ~Logger();

    void set_fileName(const QString &fileName);

    bool is_logging() const {return logging;}
    bool is_earlyLogging() const {return early_logging;}

public slots:
    void start_logging();
    void stop_logging();

protected:
    RunManager *runManager;
    QString fileName;

    QTimer *logTimer;
    bool logging;
    bool early_logging;

    virtual QVector<QString> generate_header() = 0;
};

inline void Logger::set_fileName(const QString &fileName) {this->fileName = fileName;}

#endif // LOGGER_H
