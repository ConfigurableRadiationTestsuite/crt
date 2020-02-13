#ifndef SUBWINDOW_H
#define SUBWINDOW_H

/*
 * Author: Mattis Jaksch
 *
 * SubWindows are the lowest graphical elements; they can be called
 * to be configured or to pass their configuration.
 * Alternativly items can be manually added via dialogs.
 *
 * The add-element dialog and the add-trigger dialog
 * differ as the elements are constant while the triggers dynamic
 *
 */

class ConfigElement;
class SpecTriggerDialog;

#include "src/Manager/EventManager.h"

#include <QWidget>

class SubWindow : public QWidget {
Q_OBJECT

public:
    SubWindow(EventManager *eventManager);
    virtual ~SubWindow();

    QString get_config();
    ConfigElement *get_config_element() const {return cfg_element;}

public slots:
    //Event / Trigger management
    void show_trigger_dialog();
    void add_trigger();

protected slots:
    virtual void create_layout() = 0;

    //Configuration
    virtual void post_init(); //For all the signals

signals:
    //Event / Signal management
    void signal_start();
    void signal_stop();
    void signal_event();

    void destroyed(SubWindow * subWindow);

protected:
    ConfigElement *cfg_element;

    //Event, signal and trigger management
    EventManager *eventManager;
    QVector<struct RegisteredSignal*> signal_list;

    QString get_trigger_list();
    bool is_signal_in_list(struct RegisteredSignal * reg);

    void trigger_start();
    void trigger_stop();
    void trigger_event();
};

inline void SubWindow::trigger_start() {eventManager->call_trigger(SignalType::start, signal_list);}
inline void SubWindow::trigger_stop() {eventManager->call_trigger(SignalType::stop, signal_list);}
inline void SubWindow::trigger_event() {eventManager->call_trigger(SignalType::trigger, signal_list);}

#endif // SUBWINDOW_H
