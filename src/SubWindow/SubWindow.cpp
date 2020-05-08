#include "SubWindow.h"

#include "src/Dialog/SpecSignalDialog.h"

#include <QCheckBox>
#include <QPushButton>

SubWindow::SubWindow(RunManager *runManager, Component *component)
    : runManager(runManager), component(component) {
    this->eventManager = runManager->get_eventManager();

    /* Announce logging */
    connect(this, SIGNAL(signal_start_log()), component, SLOT(start_logging()));
    eventManager->add_signal(component->get_element_name() + " Start Log", SignalType::start_log, this, &SubWindow::signal_start_log);
    connect(this, SIGNAL(signal_stop_log()), component, SLOT(stop_logging()));
    eventManager->add_signal(component->get_element_name() + " Stop Log", SignalType::stop_log, this, &SubWindow::signal_stop_log);

    /* Post config management */
    connect(eventManager, SIGNAL(signal_added()), this, SLOT(post_init()));
    connect(eventManager, SIGNAL(signal_deleted(struct RegisteredSignal *)), this, SLOT(delete_signal(struct RegisteredSignal *)));

    /* RunManager info */
    runManager->set_run_mode(AddComponent, component->get_element_name());
}

SubWindow::~SubWindow() {
    eventManager->delete_signal(&SubWindow::signal_start_log);
    eventManager->delete_signal(&SubWindow::signal_stop_log);

    runManager->set_run_mode(RemoveComponent, component->get_element_name());

    //Notify the WindowTab
    emit destroyed(this);
}

QString SubWindow::get_config() {
    component->set_config();
    component->set_value("signal", get_signal_list());

    return component->get_config();
}

QString SubWindow::get_signal_list() {
    QString list = "";

    struct RegisteredSignal *signal;
    foreach (signal, signal_list)
        list += signal->name + ";";

    return list;
}

bool SubWindow::is_signal_in_list(struct RegisteredSignal * reg) {
    RegisteredSignal * signal;
    foreach (signal, signal_list)
        if(reg == signal)
            return true;

    return false;
}

void SubWindow::post_init() {
    QString signal_names = component->get_value("signal");

    if(signal_names.count(";") == signal_list.size())
        return ;

    struct RegisteredSignal * signal;
    foreach (signal, eventManager->get_signal_list())
        if(signal_names.contains(signal->name) && !is_signal_in_list(signal))
            signal_list.push_back(signal);
}

void SubWindow::show_signal_dialog() {
    if(signalDialog != nullptr)
        delete signalDialog;

    signalDialog = new SpecSignalDialog;

    /* Check every signal thats already listed */
    struct RegisteredSignal * signal;
    foreach (signal, eventManager->get_signal_list()) {
        if(signal->st != SignalType::start_log && signal->st != SignalType::stop_log)
            is_signal_in_list(signal) ? signalDialog->add_entry(true, signal) : signalDialog->add_entry(false, signal);
    }

    connect(signalDialog, SIGNAL(accepted()), this, SLOT(add_signal()));

    signalDialog->create_dialog();
    signalDialog->show();
}

void SubWindow::add_signal() {
    struct RegisteredSignalBox signal;
    foreach (signal, signalDialog->get_registeredSignal_list())
        if(signal.checkBox->isChecked())
            signal_list.push_back(signal.sig);
}

void SubWindow::delete_signal(struct RegisteredSignal * reg) {
    for(QVector<struct RegisteredSignal*>::iterator it = signal_list.begin(); it != signal_list.end(); it++) {
        if((*it) == reg) {
            signal_list.erase(it);
            break;
        }
    }
}
