#ifndef PROGRAMMSTARTER_H
#define PROGRAMMSTARTER_H

/*
 * Author: Mattis Jaksch
 *
 */

class RunManager;

class QProcess;

#include "Component.h"

class ProgrammStarter : public Component {
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

    void execute_programm();
    void kill_programm();

    void update() override {} //Mix with receive data

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
    QProcess *process;

    QString path;
    bool running = false;
    bool trigger = false;

    void init();
    QVector<QString> generate_header() override;
};

#endif // PROGRAMMSTARTER_H
