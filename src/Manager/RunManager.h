#ifndef RUNMANAGER_H
#define RUNMANAGER_H

/*
 * Author: Mattis Jaksch
 *
 * Information on run status (not fully developed yet) and
 * run management (e.g. start/stop time and logging of components).
 *
 */

class QElapsedTimer;

#include "FileManager.h"

#include <QWidget>

class EventManager;

enum RunMode {Creation=0, Init=1, Start=3, Stop=255};

class RunManager : public QWidget, public FileManager {
Q_OBJECT

public:
    RunManager(EventManager *eventManager);
    virtual ~RunManager();

    EventManager * get_eventManager() {return eventManager;}

public slots:
    void start_run();
    void stop_run();
    void set_run_mode(enum RunMode mode);

private slots:
    void create_new_run();
    void update_run();

signals:
    void run_name_changed(const QString &);
    void run_time_changed(const QString &);
    void run_mode_changed(enum RunMode);

    void enable_run_button(bool enable);

private:
    QString runName;

    EventManager *eventManager;

    QElapsedTimer *runTime;
    QTimer * updateTime;
    long long offsetTime;

    void create_layout();
};

#endif // RUNMANAGER_H
