#include "ETHTab.h"

#include "src/Components/Ethernet.h"
#include "src/SubWindow/ETHW.h"

ETHTab::ETHTab(ConfigManager *m_configManager, RunManager *m_runManager)
    : WindowTab(m_configManager, m_runManager) {
    sectionName = "ETH";
}

void ETHTab::push_new_subwindow(const QString &config) {
    ETHW *ethw = new ETHW(runManager, new Ethernet(runManager, config));
    subWindow_list.push_back(ethw);

    layout_updater(ethw);
}

void ETHTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    uint port = addDialog->get_entry_list()[1].input_value->text().toUInt();
    long long timeout = addDialog->get_entry_list()[2].input_value->text().toULongLong();

    ETHW *ethw = new ETHW(runManager, new Ethernet(runManager, name, port, timeout));
    subWindow_list.push_back(ethw);

    layout_updater(ethw);
}

void ETHTab::create_add_subwindow_dialog() {
    addDialog->add_entry("Name", "Temperature Data");
    addDialog->add_entry("Port", "9010");
    addDialog->add_entry("Timeout", "9999");
}
