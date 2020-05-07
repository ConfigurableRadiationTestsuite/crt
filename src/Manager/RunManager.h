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
#include "EventManager.h"

#include <QWidget>

enum RunMode {Creation=0, Init=1, StartRun=3, StartLog=4, StopLog=5,
              StartComponent=6, StopComponent=7, Event=8, StopRun=255};

class RunManager : public QWidget, public FileManager {
Q_OBJECT

public:
    RunManager(EventManager *eventManager);
    virtual ~RunManager();

    EventManager * get_eventManager() {return eventManager;}

    bool is_valid() const {return valid;}
    bool is_running() const {return running;}

public slots:
    void start_run();
    void stop_run();
    void set_run_mode(enum RunMode mode, const QString &component);

private slots:
    void create_new_run();
    void update_run();

signals:
    void run_name_changed(const QString &);
    void run_time_changed(const QString &);
    void run_mode_changed(enum RunMode);

    void enable_run_button(bool enable);

    void isValid_changed(bool);
    void isInvalid_changed(bool);
    void isRunning_changed(bool);

private:
    QString runName;

    EventManager *eventManager;

    QElapsedTimer *runTime;
    QTimer * updateTime;
    long long offsetTime;

    bool valid = false;
    bool running = false;

    void create_layout();
    void set_run_valid(bool valid);
};

#endif // RUNMANAGER_H
