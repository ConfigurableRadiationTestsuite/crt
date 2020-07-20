#include "Seqtab.h"

SEQTab::SEQTab(ConfigManager *m_configManager, RunManager * m_runManager)
    : WindowTab(m_configManager, m_runManager) {
    sectionName = "SEQ";
}

void SEQTab::push_new_subwindow(const QString &config) {
/*    RFW * rfw = new RFW(runManager, new RFIO(runManager, config));
    subWindow_list.push_back(rfw);

    layout_updater(rfw);*/
}

void SEQTab::create_subwindow_from_dialog() {
    QString name = addDialog->get_entry_list()[0].input_value->text();

/*    RFW *rfw = new RFW(runManager, new RFIO(runManager, name, address, channel));
    subWindow_list.push_back(rfw);

    layout_updater(rfw);*/
}

void SEQTab::create_add_subwindow_dialog() {
    addDialog->add_entry("Name", "DUT - AD9361");
}
