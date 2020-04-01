#include "PSUTab.h"

#include "src/Components/PSU.h"
#include "src/Configuration/ConfigManager.h"
#include "src/Dialog/SpecAddDialog.h"
#include "src/Manager/EventManager.h"
#include "src/SubWindow/PSUW.h"

#include <QLineEdit>
#include <QPushButton>

PSUTab::PSUTab(ConfigManager *m_configManager, EventManager *m_eventManager, RunManager *m_runManager) : WindowTab(m_configManager, m_eventManager, m_runManager) {
    sectionName = "PSU";
}

PSUTab::~PSUTab() {}

void PSUTab::load_from_config() {
    clear_subwindow_list();

    QString section_content;
    while(configManager->get_config_section(sectionName, section_content)) {
        subWindow_list.push_back(new PSUW(new PSU(runManager, section_content), eventManager));
        section_content.clear();
    }

    update_layout();
}

void PSUTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString vendor = addDialog->get_entry_list()[1].input_value->text();
    QString address = addDialog->get_entry_list()[2].input_value->text();
    uint channel = addDialog->get_entry_list()[3].input_value->text().toUInt();
    double max_voltage = addDialog->get_entry_list()[4].input_value->text().toDouble();
    double max_current = addDialog->get_entry_list()[5].input_value->text().toDouble();

    subWindow_list.push_back(new PSUW(new PSU(runManager, name, address, vendor, channel, max_voltage, max_current), eventManager));

    update_layout();
}

void PSUTab::create_add_subwindow_dialog() {
    addDialog = new SpecAddDialog;
    addDialog->add_entry("Name", "PSU Links");
    addDialog->add_entry("Vendor", "Rohde Schwarz");
    addDialog->add_entry("Address", "192.168.3.103:5025");
    addDialog->add_entry("Channel", "3");
    addDialog->add_entry("Max Voltage [V]", "30");
    addDialog->add_entry("Max Current [mA]", "3000");

    addDialog->create_dialog();

    connect(addDialog->get_ok_button(), SIGNAL(clicked()), this, SLOT(create_subwindow_from_dialog()));
    connect(addDialog->get_ok_button(), SIGNAL(clicked()), addDialog, SLOT(close()));
}
