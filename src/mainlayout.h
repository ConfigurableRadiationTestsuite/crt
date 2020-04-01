#ifndef MAINLAYOUT_H
#define MAINLAYOUT_H

/*
 * Author: Mattis Jaksch
 *
 * Mainlayout including a menue bar and different (individual) tabs.
 * The configuration, run and event manager is passed down to the tabs in
 * order to provide a load-/saveable config and the exchange of signals
 *
 */

class ConfigManager;
class EventManager;
class RunManager;

class QAction;
class QToolBar;
class QTabWidget;
class QVBoxLayout;

#include <QWidget>

class MainLayout : public QWidget {
Q_OBJECT

public:
    MainLayout();
    virtual ~MainLayout();

private slots:
    void start_test();
    void stop_test();

private:
    ConfigManager *configManager;
    EventManager *eventManager;
    RunManager *runManager;

    QVBoxLayout *mainVerticalLayout;
    QTabWidget *windowTabs;
    QToolBar *toolbar;

    QAction* startTestButton;
    QAction* stopTestButton;

    void create_layout();

    QTabWidget * create_window_tabs();
    QToolBar * create_toolbar();
};

#endif // MAINLAYOUT_H
