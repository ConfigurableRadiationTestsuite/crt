#include "ProgrammStarter.h"

#include <QFileDialog>
#include <QProcess>

ProgrammStarter::ProgrammStarter(RunManager * runManager, const QString &config)
    : Component(runManager, config) {
    qDebug("Create ProgammStarter from Config");

    load_config(config);
    assert(parse_config({"name", "path"}));

    this->elementName = get_value("name");
    this->path = get_value("path");
    set_arguments(get_value("arguments"));

    init();
}

ProgrammStarter::ProgrammStarter(RunManager * runManager, const QString &m_element_name, const QString &path)
    : Component(m_element_name, runManager), path(path) {
    this->elementName = m_element_name;

    this->arguments = QStringList("$directory");

    init();
}

ProgrammStarter::~ProgrammStarter() {}

void ProgrammStarter::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("path", path);
    set_value("arguments", get_arguments());
}

void ProgrammStarter::init() {
    process = new QProcess(this);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(receive_data()));
}

void ProgrammStarter::set_arguments(const QString &text) {
    arguments = text.split(' ');
}

void ProgrammStarter::set_path(const QString &text) {
    if(running || !QFileInfo::exists(text))
        return;

    path = text;
    emit announce_path(path);
}

void ProgrammStarter::set_trigger(int trigger) {
    if(running)
        return ;

    this->trigger = trigger > 0 ? true : false;

    emit announce_trigger(this->trigger);
}

void ProgrammStarter::execute_programm() {
    process->start(path, substitute_arguments());
    process->waitForStarted();

    running = true;

    emit announce_run(true);
}

void ProgrammStarter::kill_programm() {
    process->kill();

    running = false;

    stop_logging();

    emit announce_run(false);
}

QStringList ProgrammStarter::generate_header() {
    return {"Line"};
}

void ProgrammStarter::receive_data() {
    QString text = process->readAllStandardOutput();

    if(text.size() > 1) {
        if(text.back() == '\n')
            text.remove(text.size()-1, 1);

        if(text.back() == '\r')
            text.remove(text.size()-1, 1);
    }

    if(logging || permanent_logging)
        runManager->append_value_to_file(this, text);

    emit data_available(text);
}

QStringList ProgrammStarter::substitute_arguments() {
    QStringList argList;

    QString element;
    foreach(element, arguments) {
        if(element.contains("$directory"))
            argList.push_back(runManager->get_root_directory());
        else
            argList.push_back(element);
    }

    return argList;
}
