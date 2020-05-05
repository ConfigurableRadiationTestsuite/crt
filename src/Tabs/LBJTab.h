#ifndef LBJTAB_H
#define LBJTAB_H

/*
 * Author: Mattis Jaksch
 *
 * Tab containing multiple labjack devices
 * with detachable plots
 *
 */

#include "src/Tabs/WindowTab.h"

class LBJTab : public WindowTab {
Q_OBJECT

public:
    LBJTab(ConfigManager *m_configManager, RunManager *m_runManager);
    virtual ~LBJTab() override;

private slots:
    void load_from_config() override;
    void create_subwindow_from_dialog() override;

private:
    void create_add_subwindow_dialog() override;
};

#endif // LBJTAB_H
