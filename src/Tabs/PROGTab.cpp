#include "PROGTab.h"

#include "src/Components/ProgrammStarter.h"
#include "src/Configuration/ConfigManager.h"
#include "src/Dialog/SpecAddDialog.h"
#include "src/Manager/EventManager.h"
#include "src/SubWindow/PROGW.h"

PROGTab::PROGTab(ConfigManager *m_configManager, RunManager *m_runManager)
    : WindowTab(m_configManager, m_runManager) {
    sectionName = "PROG";
}

PROGTab::~PROGTab() {}

void PROGTab::load_from_config() {
    clear_subwindow_list();

    QString section_content;
    while(configManager->get_config_section(sectionName, section_content)) {
        subWindow_list.push_back(new PROGW(new ProgrammStarter(runManager, section_content), runManager));
        section_content.clear();
    }

    update_layout();

}

void PROGTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString path = addDialog->get_entry_list()[1].input_value->text();

    subWindow_list.push_back(new PROGW(new ProgrammStarter(runManager, name, path), runManager));

    update_layout();
}

void PROGTab::create_add_subwindow_dialog() {
    addDialog->add_entry("Name", "Test Programm");
    addDialog->add_entry("Pfad", "../CRT/CRTTest");
}


