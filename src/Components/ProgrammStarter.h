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

private:
    QString path;
    bool is_early_logging;
    bool is_running;
    bool is_trigger;

    QProcess * process;
};

#endif // PROGRAMMSTARTER_H
