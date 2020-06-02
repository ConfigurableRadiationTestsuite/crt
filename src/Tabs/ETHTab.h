#ifndef ETHTAB_H
#define ETHTAB_H

/*
 * Author: Mattis Jaksch
 *
 */

#include "src/Tabs/WindowTab.h"

class ETHTab : public WindowTab {
Q_OBJECT

public:
    ETHTab(ConfigManager *m_configManager, RunManager *m_runManager);
    virtual ~ETHTab() override;

private slots:
    void load_from_config() override;
    void create_subwindow_from_dialog() override;

private:
    void create_add_subwindow_dialog() override;
};

#endif // ETHTAB_H
