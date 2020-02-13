#include "EventManager.h"

EventManager::EventManager() {}

EventManager::~EventManager() {}

void EventManager::add_signal(const QString &name, SignalType st, SubWindow *sub, void (SubWindow::*sp)(void)) {
    signal_list.push_back(new RegisteredSignal{name, st, sub, sp});

    emit signal_added();
}

