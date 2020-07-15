#ifndef PROGRAMMSTARTER_H
#define PROGRAMMSTARTER_H

/*
 * Author: Mattis Jaksch
 *
 * A path to an exection program can be specified;
 * The output is logged into a file
 *
 */

class QProcess;

#include "Component.h"

class ProgrammStarter : public Component {
Q_OBJECT

public:
    ProgrammStarter(RunManager * runManager, const QString &config);
    ProgrammStarter(RunManager * runManager, const QString &m_element_name, const QString &path);

    void set_config() override;

    QString get_path() const {return path;}
    QString get_arguments();

public slots:
    void set_arguments(const QString &text);
    void set_path(const QString &text);
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
    QStringList arguments;
    bool running = false;
    bool trigger = false;

    void init();
    QStringList generate_header() override;

    QStringList substitute_arguments();
};

inline QString ProgrammStarter::get_arguments() {
    return arguments.join(' ');
}

inline QStringList ProgrammStarter::generate_header() {
    return {"Line"};
}

inline void ProgrammStarter::set_arguments(const QString &text) {
    arguments = text.split(' ');
}

#endif // PROGRAMMSTARTER_H
