#include "FFOXTab.h"

#include "src/Components/Fieldfox.h"
#include "src/SubWindow/FFOXW.h"

#include "LabJackM.h"

FFOXTab::FFOXTab(ConfigManager* m_configManager, RunManager* m_runManager)
    : WindowTab(m_configManager, m_runManager)
{
    sectionName = "Fieldfox";
}

void FFOXTab::push_new_subwindow(const QString& config)
{
    FFOXW* ffoxw = new FFOXW(runManager, new Fieldfox(runManager, config));
    subWindow_list.push_back(ffoxw);

    layout_updater(ffoxw);
}

void FFOXTab::create_subwindow_from_dialog()
{
    QString name = addDialog->get_entry_list()[0].input_value->text();
    QString address = addDialog->get_entry_list()[1].input_value->text();

    QString mode = addDialog->get_entry_list()[2].input_value->text();

    //Delete spaces and convert
    ulong start_freq = addDialog->get_entry_list()[3].input_value->text().remove(" ").toULong();
    ulong stop_freq = addDialog->get_entry_list()[4].input_value->text().remove(" ").toULong();
    ulong points = addDialog->get_entry_list()[5].input_value->text().remove(" ").toULong();

    FFOXW* ffoxw = new FFOXW(runManager, new Fieldfox(runManager, name, address, mode, start_freq, stop_freq, points));
    subWindow_list.push_back(ffoxw);

    layout_updater(ffoxw);
}

void FFOXTab::create_add_subwindow_dialog()
{
    addDialog->add_entry("Name", "Fieldfox N");
    addDialog->add_entry("Address", "192.168.3.141:5025");

    addDialog->add_entry("Mode", "Unused");
    addDialog->get_entry_list()[2].input_value->setReadOnly(true);

    addDialog->add_entry("Start Frequency", "2400 991 500");
    addDialog->add_entry("Stop Frequency", "2401 011 500");
    addDialog->add_entry("Points", "20001");
}
