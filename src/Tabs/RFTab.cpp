#include "RFTab.h"

#include "src/Configuration/ConfigManager.h"
#include "src/Manager/EventManager.h"
#include "src/Dialog/SpecAddDialog.h"
#include "src/SubWindow/RFW.h"
#include "src/Components/RFIO.h"

RFTab::RFTab(ConfigManager *m_configManager, EventManager *m_eventManager, RunManager *m_runManager) : WindowTab(m_configManager, m_eventManager, m_runManager) {
    sectionName = "RF";
}

RFTab::~RFTab() {}

void RFTab::load_from_config() {
    clear_subwindow_list();

    QString section_content;
    while(configManager->get_config_section(sectionName, section_content)) {
        subWindow_list.push_back(new RFW(new RFIO(runManager, section_content), eventManager));
        section_content.clear();
    }

    update_layout();
}

void RFTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString address = addDialog->get_entry_list()[1].input_value->text();
    int channel = addDialog->get_entry_list()[2].input_value->text().toInt();

    subWindow_list.push_back(new RFW(new RFIO(runManager, name, address, channel), eventManager));

    update_layout();
}

void RFTab::create_add_subwindow_dialog() {
    addDialog->add_entry("Name", "DUT - AD9361");
    addDialog->add_entry("Address", "192.168.3.2:9001");
    addDialog->add_entry("Channel", "2");
}
