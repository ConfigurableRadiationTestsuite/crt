#ifndef SUBWINDOW_H
#define SUBWINDOW_H

/*
 * Author: Mattis Jaksch
 *
 * SubWindows are the lowest graphical elements; they can be called
 * to be configured or to pass their configuration.
 * As alternative, items can be manually added via dialogs.
 *
 * The add-element dialog has to be created locally by the
 * subwindows, while the add-trigger dialog can be created globally
 *
 */

class ConfigElement;
class SpecSignalDialog;

#include "src/Manager/RunManager.h"

#include <QWidget>

class SubWindow : public QWidget {
Q_OBJECT

public:
    SubWindow(RunManager *runManager);
    virtual ~SubWindow();

    QString get_config();
    ConfigElement *get_config_element() const {return cfg_element;}

public slots:
    /* Event / Signal management */
    void show_signal_dialog();
    void add_signal();
    void delete_signal(struct RegisteredSignal * reg);

protected slots:
    virtual void create_layout() = 0;

    //Configuration
    virtual void post_init(); //For all the signals

    /* Emit listed signals */
    void trigger_on();
    void trigger_off();
    void trigger_start_start();
    void trigger_stop_stop();
    void trigger_special();
    void trigger_signal_list();

signals:
    /* Event / Signal management */
    void signal_on();
    void signal_off();
    void signal_start_log();
    void signal_stop_log();
    void signal_event();

    //Override for WindowTab
    void destroyed(SubWindow * subWindow);

protected:
    ConfigElement *cfg_element;

    /* Eventand signal management */
    RunManager *runManager;
    EventManager *eventManager;
    QVector<struct RegisteredSignal*> signal_list;
    SpecSignalDialog *signalDialog = nullptr;

    QString get_signal_list();
    bool is_signal_in_list(struct RegisteredSignal * reg);
};

inline void SubWindow::trigger_on() {eventManager->call_trigger(SignalType::on, signal_list);}
inline void SubWindow::trigger_off() {eventManager->call_trigger(SignalType::off, signal_list);}
inline void SubWindow::trigger_start_start() {eventManager->call_trigger(SignalType::start_log, signal_list);}
inline void SubWindow::trigger_stop_stop() {eventManager->call_trigger(SignalType::stop_log, signal_list);}
inline void SubWindow::trigger_special() {eventManager->call_trigger(SignalType::special, signal_list);}
inline void SubWindow::trigger_signal_list() {eventManager->call_trigger(signal_list);}

#endif // SUBWINDOW_H
