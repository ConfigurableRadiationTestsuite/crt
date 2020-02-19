#include "LBJTab.h"

#include "src/Configuration/ConfigManager.h"
#include "src/Manager/EventManager.h"
#include "src/Dialog/SpecAddDialog.h"
#include "src/Components/Labjack.h"
#include "src/SubWindow/LBJW.h"

#include <QLineEdit>
#include <QPushButton>

LBJTab::LBJTab(ConfigManager *m_configManager, EventManager *m_eventManager, RunManager *m_runManager) : WindowTab(m_configManager, m_eventManager, m_runManager) {
    sectionName = "LabJack";
}

LBJTab::~LBJTab() {}

void LBJTab::load_from_config() {
    clear_subwindow_list();

    QString section_content;
    while(configManager->get_config_section(sectionName, section_content)) {
        subWindow_list.push_back(new LBJW(new Labjack(runManager, section_content), eventManager));
        section_content.clear();
    }

    update_layout();
}

void LBJTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    //QString type = addDialog->get_entry_list()[1].input_value->text();
    QString channel_name = addDialog->get_entry_list()[1].input_value->text();
    QString pchannel = addDialog->get_entry_list()[2].input_value->text();
    QString nchannel = addDialog->get_entry_list()[3].input_value->text();

    subWindow_list.push_back(new LBJW(new Labjack(runManager, name, channel_name, pchannel, nchannel), eventManager));

    update_layout();
}

void LBJTab::create_add_subwindow_dialog() {
    addDialog = new SpecAddDialog;
    addDialog->add_entry("Name", "Rotes T7");
    //addDialog->add_entry("Type", "T7");
    addDialog->add_entry("Channel Name", "AIN62/68, AIN63");
    addDialog->add_entry("Channel Positive", "62, 63");
    addDialog->add_entry("Channel Negative", "68, 199");

    addDialog->create_dialog();

    connect(addDialog->get_ok_button(), SIGNAL(clicked()), this, SLOT(create_subwindow_from_dialog()));
    connect(addDialog->get_ok_button(), SIGNAL(clicked()), addDialog, SLOT(close()));
}
