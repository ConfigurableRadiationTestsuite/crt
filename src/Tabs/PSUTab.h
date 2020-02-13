#ifndef PSUTAB_H
#define PSUTAB_H

/*
 * Author: Mattis Jaksch
 *
 * Tab containing multiple powersupplies.
 * They can be either added via button and dialog
 * or with a provided configuration file.
 *
 */

#include "src/Tabs/WindowTab.h"

class PSUTab : public WindowTab {
Q_OBJECT

public:
    PSUTab(ConfigManager *m_configManager, EventManager *m_eventManager, RunManager *m_runManager);
    virtual ~PSUTab() override;

private slots:
    void load_from_config() override;
    void create_subwindow_from_dialog() override;

private:
    void create_add_subwindow_dialog() override;
};

#endif // PSUTAB_H
