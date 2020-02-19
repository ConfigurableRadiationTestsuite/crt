#ifndef RUNMANAGER_H
#define RUNMANAGER_H

class QElapsedTimer;

#include "FileManager.h"

#include <QWidget>

enum RunMode {Creation=0, Init=1, Start=3, Stop=255};

class RunManager : public QWidget, public FileManager{
Q_OBJECT

public:
    RunManager();
    virtual ~RunManager();

public slots:
    void start_run();
    void stop_run();

private slots:
    void create_new_run();
    void update_run();

signals:
    void run_name_changed(const QString &text);
    void run_time_changed(const QString &text);

    bool enable_run_button(bool enable);

private:
    QString runName;

    QElapsedTimer *runTime;
    QTimer * updateTime;
    long long offsetTime;

    void create_layout();
};

#endif // RUNMANAGER_H
