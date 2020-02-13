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

signals:
    void signal_added();

private:
    QVector<struct RegisteredSignal*> signal_list;

};

#endif // EVENTMANAGER_H
