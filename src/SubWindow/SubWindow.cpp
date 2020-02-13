#include "SubWindow.h"

#include "src/Configuration/ConfigElement.h"

SubWindow::SubWindow(EventManager *eventManager) : eventManager(eventManager) {
    connect(eventManager, SIGNAL(signal_added()), this, SLOT(post_init()));
}

SubWindow::~SubWindow() {
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
    QString signal_names = cfg_element->get_value("signal");

    if(signal_names.count(";") == signal_list.size())
        return ;

    struct RegisteredSignal * signal;
    foreach (signal, eventManager->get_signal_list())
        if(signal_names.contains(signal->name) && !is_signal_in_list(signal))
            signal_list.push_back(signal);
}

void SubWindow::show_trigger_dialog() {
    //TODO
}

void SubWindow::add_trigger() {
    //TODO
}
