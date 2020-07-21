#include "EventManager.h"

EventManager::EventManager() {}

EventManager::~EventManager() {}

void EventManager::add_signal(const QString &name, SignalType st, SubWindow *sub, void (SubWindow::*sp)(void)) {
    signal_list.push_back(new RegisteredSignal{name, st, sub, sp});
    emit signal_added();
}

void EventManager::delete_signal(void (SubWindow::*sp)(void)) {
    for(int i = 0; i < signal_list.size(); i++) {
        if(signal_list[i]->sp == sp) {
            emit signal_deleted(signal_list[i]);

            delete signal_list[i];
            signal_list.remove(i);

            return ;
        }
    }
}

void EventManager::call_trigger(const QVector<struct RegisteredSignal*> &signal_list) {
    foreach (RegisteredSignal * signal, signal_list)
            emit ((signal->sub)->*(signal->sp))();
}

void EventManager::call_trigger(enum SignalType st, const QVector<struct RegisteredSignal*> &signal_list) {
    foreach (RegisteredSignal * signal, signal_list)
        if(signal->st == st)
            emit ((signal->sub)->*(signal->sp))();
}

RegisteredSignal * EventManager::get_signal(const QString &name) const {
    foreach (RegisteredSignal *signal, signal_list)
        if(signal->name == name)
            return signal;

    return nullptr;
}
