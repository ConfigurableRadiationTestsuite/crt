#include "ETHW.h"

#include "src/Components/Ethernet.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

ETHW::ETHW(RunManager *m_runManager, Ethernet *ethernet)
    : SubWindow(m_runManager, ethernet), ethernet(ethernet) {

    /* Connect and register signals */
/*    connect(this, SIGNAL(signal_on()), ethernet, SLOT(switch_on()));
    eventManager->add_signal(psu->get_element_name() + " Switch On", SignalType::on, this, &SubWindow::signal_on);

    connect(this, SIGNAL(signal_off()), psu, SLOT(switch_off()));
    eventManager->add_signal(psu->get_element_name() + " Switch Off", SignalType::off, this, &SubWindow::signal_off);
*/
    create_layout();
}

ETHW::~ETHW() {
    /* Degregister signals */
    eventManager->delete_signal(&SubWindow::signal_on);
    eventManager->delete_signal(&SubWindow::signal_off);

    delete ethernet;
}

void ETHW::create_layout() {
    QGridLayout *mainGridLayout = new QGridLayout(this);

    /* Name */
    QLabel *nameLabel = new QLabel("Name");
    QLineEdit *nameEdit = new QLineEdit;
    //setText()
    mainGridLayout->addWidget(nameLabel, 0, 0);
    mainGridLayout->addWidget(nameEdit, 1, 0);

    /* Address */
    QLabel *addressLabel = new QLabel("Address");
    QLineEdit *addressEdit = new QLineEdit;
    //setText()
    mainGridLayout->addWidget(addressLabel, 0, 1);
    mainGridLayout->addWidget(addressEdit, 1, 1);

    /* Port */
    QLabel *portLabel = new QLabel("Port");
    QLineEdit *portEdit = new QLineEdit;
    //setText()
    mainGridLayout->addWidget(portLabel, 0, 2);
    mainGridLayout->addWidget(portEdit, 1, 2);

    /* Received Bytes */
    QLabel *byteLabel = new QLabel("Bytes");
    QLineEdit *byteEdit = new QLineEdit;
    //setText()
    mainGridLayout->addWidget(byteLabel, 0, 3);
    mainGridLayout->addWidget(byteEdit, 1, 3);

    /* Received Files */
    QLabel *fileLabel = new QLabel("Files");
    QLineEdit *fileEdit = new QLineEdit;
    //setText()
    mainGridLayout->addWidget(fileLabel, 0, 4);
    mainGridLayout->addWidget(fileEdit, 1, 4);

    /* Activity Indicator */
    QLabel *activeLabel = new QLabel("Active");
    //setText()
    mainGridLayout->addWidget(activeLabel, 0, 5);
//        /* Enable */
//        Indicator * enableBox = new Indicator(QIcon("../CRT/icon/PSU_on.png"), QIcon("../CRT/icon/PSU_off.png"), QIcon("../CRT/icon/PSU_disconnected.png"));
//        psuGridLayout->addWidget(enableBox, 0, 2);
//        connect(enableBox, SIGNAL(stateChanged(int)), channel, SLOT(set_enable(int)));
//        connect(channel, SIGNAL(enable_changed(bool)), enableBox, SLOT(setChecked(bool)));
//        connect(psu, SIGNAL(disconnected(bool)), enableBox, SLOT(set_event_icon(bool)));

    setLayout(mainGridLayout);
}
