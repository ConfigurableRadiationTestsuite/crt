#ifndef WINDOWTAB_H
#define WINDOWTAB_H

/*
 * Author: Mattis Jaksch
 *
 * Abstract class for generating tabs included in the mainlayout.
 * Tabs can contain (abstract) SubWindows, which are configurable by
 * a config passed down from the ConfigManager
 *
 */

class EventManager;
class ConfigManager;
class RunManager;
class SpecAddDialog;
class SubWindow;

class QVBoxLayout;

#include <QWidget>

class WindowTab : public QWidget {
Q_OBJECT

public:
    WindowTab(ConfigManager *configManager, EventManager *eventManager, RunManager *runManager);
    virtual ~WindowTab();

protected slots:
    virtual void create_layout();

    virtual void load_from_config() = 0;
    void save_to_config();

    virtual void create_subwindow_from_dialog() = 0;
    void add_subwindow_from_dialog();

    void update_layout();

private slots:
    void erase_window(SubWindow * window);

signals:
    void clean_layout();

protected:
    QString sectionName;

    ConfigManager *configManager;
    EventManager *eventManager;
    RunManager *runManager;

    QVBoxLayout *mainVerticalLayout;

    QList<SubWindow *> subWindow_list;

    SpecAddDialog *addDialog = nullptr;

    void clear_subwindow_list();
    void clear_layout(QLayout *layout);

    virtual void create_add_subwindow_dialog() = 0;
};

#endif // WINDOWTAB_H
