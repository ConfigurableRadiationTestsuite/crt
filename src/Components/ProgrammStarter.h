#ifndef PROGRAMMSTARTER_H
#define PROGRAMMSTARTER_H

/*
 * Author: Mattis Jaksch
 *
 */

class RunManager;

class QProcess;

#include "src/Configuration/ConfigElement.h"

#include <QObject>

class ProgrammStarter : public QObject, public ConfigElement {
Q_OBJECT

public:
    ProgrammStarter(RunManager * runManager, const QString &config);
    ProgrammStarter(RunManager * runManager, const QString &m_element_name, const QString &path);
    virtual ~ProgrammStarter();

    void set_config() override;

    QString get_path() const {return path;}

public slots:
    void set_path(const QString &text);
    void set_early_logging(int early_logging);
    void set_trigger(int trigger);

    void start_logging();
    void stop_logging();

    void execute_programm();
    void kill_programm();

private slots:
    void receive_data();

signals:
    void data_available(const QString &);
    void started();
    void stopped();

    void announce_trigger(bool);
    void announce_run(bool);
    void announce_path(const QString &);

private:
    RunManager *runManager;
    QProcess *process;

    QString path;
    bool is_early_logging = false;
    bool is_logging = false;
    bool is_running = false;
    bool is_trigger = false;

    void init();
};

#endif // PROGRAMMSTARTER_H
