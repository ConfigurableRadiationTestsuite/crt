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
            delete signal_list[i];
            signal_list.remove(i);
            return ;
        }
    }
}

void EventManager::call_trigger(enum SignalType st, const QVector<struct RegisteredSignal*> &signal_list) {
    RegisteredSignal * signal;
    foreach (signal, signal_list)
        if(signal->st == st)
            emit ((signal->sub)->*(signal->sp))();
}
