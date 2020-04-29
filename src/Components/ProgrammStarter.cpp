#include "ProgrammStarter.h"

#include <QProcess>

ProgrammStarter::ProgrammStarter(RunManager * runManager, const QString &config) {

}

ProgrammStarter::ProgrammStarter(RunManager * runManager, const QString &m_element_name, const QString &path) {
    this->element_name = m_element_name;
    this->path = path;

    process = new QProcess;
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(receive_data()));
}

ProgrammStarter::~ProgrammStarter() {}

void ProgrammStarter::set_config() {

}

void ProgrammStarter::set_path(const QString &text) {
    if(is_running)
        return;

    //Check if path exists

    path = text;

    //emit signal
}

void ProgrammStarter::set_early_logging(int early_logging) {
    is_early_logging = early_logging > 0 ? true : false;

    //emit signal
}

void ProgrammStarter::set_trigger(int trigger) {
    if(is_running)
        return ;

    is_trigger = trigger > 0 ? true : false;

    emit announce_trigger(is_trigger);
}

void ProgrammStarter::start_logging() {

}

void ProgrammStarter::stop_logging() {

}

void ProgrammStarter::execute_programm() {
    process->start(path);
    process->waitForStarted();

    is_running = true;
    emit announce_run(true);
}

void ProgrammStarter::kill_programm() {

    process->kill();

    is_running = false;
    emit announce_run(false);
}

void ProgrammStarter::receive_data() {
    QString text = process->readAllStandardOutput();

    if(text.size() > 1) {
        if(text[text.size()-1] == '\n')
            text.remove(text.size()-1, 1);

        if(text[text.size()-1] == '\r')
            text.remove(text.size()-1, 1);
    }

    emit data_available(text);
}
