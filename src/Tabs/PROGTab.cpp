#include "PROGTab.h"

#include "src/Components/ProgrammStarter.h"
#include "src/SubWindow/PROGW.h"

PROGTab::PROGTab(ConfigManager *m_configManager, RunManager *m_runManager)
    : WindowTab(m_configManager, m_runManager) {
    sectionName = "PROG";
}

PROGTab::~PROGTab() {}

void PROGTab::push_new_subwindow(const QString &config) {
    subWindow_list.push_back(new PROGW(runManager, new ProgrammStarter(runManager, config)));
}

void PROGTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString path = addDialog->get_entry_list()[1].input_value->text();

    subWindow_list.push_back(new PROGW(runManager, new ProgrammStarter(runManager, name, path)));

    update_layout();
}

void PROGTab::create_add_subwindow_dialog() {
    addDialog->add_entry("Name", "Test Programm");
    addDialog->add_entry("Pfad", "../CRT/CRTTest");
}


