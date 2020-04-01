#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

/*
 * Author: Mattis Jaksch
 *
 * Allows distribution of signals
 * to different objects.
 *
 */

class SubWindow;

#include <QWidget>

enum SignalType {on, off, start_log, stop_log, trigger};

struct RegisteredSignal {
    QString name;
    SignalType st;

    SubWindow *sub;
    void (SubWindow::*sp)(void);
};

class EventManager : public QWidget {
Q_OBJECT

public:
    EventManager();
    virtual ~EventManager();

    QVector<struct RegisteredSignal*> get_signal_list() {return signal_list;}

    void add_signal(const QString &name, SignalType st, SubWindow *sub, void (SubWindow::*sp)(void));
    void delete_signal(void (SubWindow::*sp)(void));

public slots:
    void trigger_on();
    void trigger_off();
    void trigger_start_log();
    void trigger_stop_log();
    void trigger_event();

    void call_trigger(const QVector<struct RegisteredSignal*> &signal_list);
    void call_trigger(enum SignalType st, const QVector<struct RegisteredSignal*> &signal_list);

signals:
    void signal_added();
    void signal_deleted(struct RegisteredSignal * reg);

private:
    QVector<struct RegisteredSignal*> signal_list;

};

inline void EventManager::trigger_on() {call_trigger(SignalType::on, signal_list);}
inline void EventManager::trigger_off() {call_trigger(SignalType::off, signal_list);}
inline void EventManager::trigger_start_log() {call_trigger(SignalType::start_log, signal_list);}
inline void EventManager::trigger_stop_log() {call_trigger(SignalType::stop_log, signal_list);}
inline void EventManager::trigger_event() {call_trigger(SignalType::trigger, signal_list);}

#endif // EVENTMANAGER_H
