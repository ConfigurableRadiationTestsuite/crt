#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

/*
 * Author: Mattis Jaksch
 *
 * Allows distribution of signals
 * to different objects
 *
 */

class SubWindow;

#include <QWidget>

enum SignalType {start, stop, trigger};

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
    void trigger_start();
    void trigger_stop();
    void trigger_event();

    void call_trigger(enum SignalType st, const QVector<struct RegisteredSignal*> &signal_list);

signals:
    void signal_added();

private:
    QVector<struct RegisteredSignal*> signal_list;

};

inline void EventManager::trigger_start() {call_trigger(SignalType::start, signal_list);}
inline void EventManager::trigger_stop() {call_trigger(SignalType::stop, signal_list);}
inline void EventManager::trigger_event() {call_trigger(SignalType::trigger, signal_list);}

#endif // EVENTMANAGER_H
