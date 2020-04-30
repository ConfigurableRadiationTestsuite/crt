#include "ProgrammStarter.h"

#include "src/Manager/RunManager.h"

#include <QFileDialog>
#include <QProcess>

ProgrammStarter::ProgrammStarter(RunManager * runManager, const QString &config)
    : runManager(runManager) {
    qDebug("Create ProgammStarter from Config");

    load_config(config);
    assert(parse_config({"name", "path"}));

    this->element_name = get_value("name");
    this->path = get_value("path");

    init();
}

ProgrammStarter::ProgrammStarter(RunManager * runManager, const QString &m_element_name, const QString &path)
    : runManager(runManager), path(path) {
    this->element_name = m_element_name;

    init();
}

ProgrammStarter::~ProgrammStarter() {
    qDebug("Destroy ProgrammStarter");
    delete process;
}

void ProgrammStarter::set_config() {
    config_entry_list.clear();

    set_value("name", element_name);
    set_value("path", path);
}

void ProgrammStarter::init() {
    process = new QProcess;
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(receive_data()));
}

void ProgrammStarter::set_path(const QString &text) {
    if(is_running || !QFileInfo::exists(text))
        return;

    path = text;
    emit announce_path(path);
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
    if(is_logging)
        return;

    runManager->register_component(this, element_name);
    runManager->set_file_header(this, {"Line"});

    is_logging = true;
}

void ProgrammStarter::stop_logging() {
    if(!is_logging)
        return ;

    runManager->deregister_component(this);
    is_logging = false;
}

void ProgrammStarter::execute_programm() {
    if(is_early_logging)
        start_logging();

    process->start(path);
    process->waitForStarted();

    is_running = true;

    emit announce_run(true);
}

void ProgrammStarter::kill_programm() {
    process->kill();

    is_running = false;

    stop_logging();

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

    if(is_logging)
        runManager->append_value_to_file(this, text);

    emit data_available(text);
}
