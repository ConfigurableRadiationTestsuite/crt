#include "PROGTab.h"

#include "src/Components/ProgrammStarter.h"
#include "src/Configuration/ConfigManager.h"
#include "src/Dialog/SpecAddDialog.h"
#include "src/Manager/EventManager.h"
#include "src/SubWindow/PROGW.h"

#include <QLineEdit>
#include <QPushButton>

PROGTab::PROGTab(ConfigManager *m_configManager, EventManager *m_eventManager, RunManager *m_runManager)
    : WindowTab(m_configManager, m_eventManager, m_runManager) {
    sectionName = "PROG";
}

PROGTab::~PROGTab() {}

void PROGTab::load_from_config() {
    clear_subwindow_list();

    QString section_content;
    while(configManager->get_config_section(sectionName, section_content)) {
        subWindow_list.push_back(new PROGW(new ProgrammStarter(runManager, section_content), eventManager));
        section_content.clear();
    }

    update_layout();

}

void PROGTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString path = addDialog->get_entry_list()[1].input_value->text();

    subWindow_list.push_back(new PROGW(new ProgrammStarter(runManager, name, path), eventManager));

    update_layout();
}

void PROGTab::create_add_subwindow_dialog() {
    addDialog = new SpecAddDialog;
    addDialog->add_entry("Name", "Test Programm");
    addDialog->add_entry("Pfad", "/usr/bin/sh");

    addDialog->create_dialog();

    connect(addDialog->get_ok_button(), SIGNAL(clicked()), this, SLOT(create_subwindow_from_dialog()));
    connect(addDialog->get_ok_button(), SIGNAL(clicked()), addDialog, SLOT(close()));
}


