#include "SubWindow.h"

#include "src/Configuration/ConfigElement.h"
#include "src/Dialog/SpecTriggerDialog.h"

#include <QPushButton>
#include <QCheckBox>

SubWindow::SubWindow(EventManager *eventManager) : eventManager(eventManager) {
    qDebug("Create subwindow");

    connect(eventManager, SIGNAL(signal_added()), this, SLOT(post_init()));
    connect(eventManager, SIGNAL(signal_deleted(struct RegisteredSignal *)), this, SLOT(delete_trigger(struct RegisteredSignal *)));
}

SubWindow::~SubWindow() {
    qDebug("Destroy SubWindow: " + (cfg_element->get_element_name()).toLatin1());
    emit destroyed(this);
}

QString SubWindow::get_config() {
    cfg_element->set_config();
    cfg_element->set_value("signal", get_trigger_list());

    return cfg_element->get_config();
}

QString SubWindow::get_trigger_list() {
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
    qDebug("Call post init on: " + (cfg_element->get_element_name()).toLatin1());

    QString signal_names = cfg_element->get_value("signal");

    if(signal_names.count(";") == signal_list.size())
        return ;

    struct RegisteredSignal * signal;
    foreach (signal, eventManager->get_signal_list())
        if(signal_names.contains(signal->name) && !is_signal_in_list(signal))
            signal_list.push_back(signal);
}

void SubWindow::show_trigger_dialog() {
    if(triggerDialog != nullptr)
        delete triggerDialog;

    triggerDialog = new SpecTriggerDialog;

    struct RegisteredSignal * signal;
    foreach (signal, eventManager->get_signal_list()) {
        if(is_signal_in_list(signal))
            triggerDialog->add_entry(true, signal);
        else
            triggerDialog->add_entry(false, signal);
    }

    connect(triggerDialog->get_ok_button(), SIGNAL(clicked()), this, SLOT(add_trigger()));
    connect(triggerDialog->get_ok_button(), SIGNAL(clicked()), triggerDialog, SLOT(close()));


    triggerDialog->create_dialog();
    triggerDialog->show();
}

void SubWindow::add_trigger() {
    qDebug("Add trigger to: " + (cfg_element->get_element_name()).toLatin1());

    struct RegisteredSignalBox signal;
    foreach (signal, triggerDialog->get_registeredSignal_list())
        if(signal.checkBox->isChecked())
            signal_list.push_back(signal.sig);
}

void SubWindow::delete_trigger(struct RegisteredSignal * reg) {
    qDebug("Delete trigger from: " + (cfg_element->get_element_name()).toLatin1());

    for(QVector<struct RegisteredSignal*>::iterator it = signal_list.begin(); it != signal_list.end(); it++) {
        if((*it) == reg) {
            signal_list.erase(it);
            break;
        }
    }
}