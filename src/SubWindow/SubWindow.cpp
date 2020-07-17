#include "SubWindow.h"

#include "src/Dialog/SpecSignalDialog.h"

#include <QCheckBox>
#include <QPushButton>
#include <QThread>

SubWindow::SubWindow(RunManager *runManager, Component *component)
    : runManager(runManager), component(component) {

    this->eventManager = runManager->get_eventManager();

#ifdef MULTITHREAD_TEST
    thread = new QThread;
    component->moveToThread(thread);

    connect(thread, SIGNAL(started()), component, SLOT(configure_timer()));

    thread->start();
#endif

    /* Announce logging */
    connect(this, SIGNAL(signal_start_log()), component, SLOT(start_logging()));
    eventManager->add_signal(component->get_element_name() + " Start Log", SignalType::start_log, this, &SubWindow::signal_start_log);

    connect(this, SIGNAL(signal_stop_log()), component, SLOT(stop_logging()));
    eventManager->add_signal(component->get_element_name() + " Stop Log", SignalType::stop_log, this, &SubWindow::signal_stop_log);

    /* Post config management */
    connect(eventManager, SIGNAL(signal_added()), this, SLOT(post_init()));
    connect(eventManager, SIGNAL(signal_deleted(struct RegisteredSignal *)), this, SLOT(delete_signal(struct RegisteredSignal *)));
}

SubWindow::~SubWindow() {
    eventManager->delete_signal(&SubWindow::signal_start_log);
    eventManager->delete_signal(&SubWindow::signal_stop_log);

#ifdef MULTITHREAD_TEST
    thread->quit();
#endif

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

    foreach (struct RegisteredSignal *signal, signal_list)
        list += signal->name + ";";

    return list;
}

void SubWindow::post_init() {
    QString signal_names = component->get_value("signal");

    if(signal_names.count(";") == signal_list.size())
        return ;

    foreach (struct RegisteredSignal * signal, eventManager->get_signal_list())
        if(signal_names.contains(signal->name) && !is_signal_in_list(signal))
            signal_list.push_back(signal);
}

void SubWindow::show_signal_dialog() {
    if(signalDialog != nullptr)
        delete signalDialog;

    signalDialog = new SpecSignalDialog;

    /* Check every signal thats already listed */
    foreach (struct RegisteredSignal * signal, eventManager->get_signal_list()) {
        if(signal->st != SignalType::start_log && signal->st != SignalType::stop_log)
            is_signal_in_list(signal) ? signalDialog->add_entry(true, signal) : signalDialog->add_entry(false, signal);
    }

    connect(signalDialog, SIGNAL(accepted()), this, SLOT(add_signal()));

    signalDialog->create_dialog();
    signalDialog->show();
}

void SubWindow::add_signal() {
    foreach (RegisteredSignalBox signal, signalDialog->get_registeredSignal_list())
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
