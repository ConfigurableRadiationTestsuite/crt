#ifndef FFOXTAB_H
#define FFOXTAB_H

/*
 * Author: Mattis Jaksch
 *
 */

#include "src/Tabs/WindowTab.h"

class FFOXTab : public WindowTab
{
Q_OBJECT

public:
    FFOXTab(ConfigManager* m_configManager, RunManager* m_runManager);

private slots:
    void push_new_subwindow(const QString& config) override;
    void create_subwindow_from_dialog() override;

private:
    void create_add_subwindow_dialog() override;
};

#endif // FFOXTAB_H
