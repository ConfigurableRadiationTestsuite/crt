#include "RFTab.h"

#include "src/Components/RFIO.h"
#include "src/SubWindow/RFW.h"

RFTab::RFTab(ConfigManager *m_configManager, RunManager *m_runManager)
    : WindowTab(m_configManager, m_runManager) {
    sectionName = "RF";
}

void RFTab::push_new_subwindow(const QString &config) {
    subWindow_list.push_back(new RFW(runManager, new RFIO(runManager, config)));
}

void RFTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString address = addDialog->get_entry_list()[1].input_value->text();
    int channel = addDialog->get_entry_list()[2].input_value->text().toInt();

    subWindow_list.push_back(new RFW(runManager, new RFIO(runManager, name, address, channel)));

    update_layout();
}

void RFTab::create_add_subwindow_dialog() {
    addDialog->add_entry("Name", "DUT - AD9361");
    addDialog->add_entry("Address", "192.168.3.2:9001");
    addDialog->add_entry("Channel", "2");
}
