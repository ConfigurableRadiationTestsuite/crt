#ifndef RFTAB_H
#define RFTAB_H

/*
 * Author: Mattis Jaksch
 *
 * Tab containing an iio daemon to read from devices
 * and plot the RF data
 *
 */

#include "src/Tabs/WindowTab.h"

class RFTab : public WindowTab {
Q_OBJECT

public:
    RFTab(ConfigManager *m_configManager, RunManager *m_runManager);
    virtual ~RFTab() override;

private slots:
    void load_from_config() override;
    void create_subwindow_from_dialog() override;

private:
    void create_add_subwindow_dialog() override;
};

#endif // LBJTAB_H
