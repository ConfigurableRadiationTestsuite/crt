#include "ETHW.h"

#include "src/Components/Ethernet.h"
#include "src/Components/IndicatorIcon.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

ETHW::ETHW(RunManager *m_runManager, Ethernet *ethernet)
    : SubWindow(m_runManager, ethernet), ethernet(ethernet) {

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
    QLineEdit *nameEdit = new QLineEdit(ethernet->get_element_name());
    nameEdit->setReadOnly(true);
    mainGridLayout->addWidget(nameLabel, 0, 0);
    mainGridLayout->addWidget(nameEdit, 1, 0);

    /* Address */
    QLabel *addressLabel = new QLabel("Address");
    QLineEdit *addressEdit = new QLineEdit;
    addressEdit->setReadOnly(true);
    mainGridLayout->addWidget(addressLabel, 0, 1);
    mainGridLayout->addWidget(addressEdit, 1, 1);

    /* Port */
    QLabel *portLabel = new QLabel("Port");
    QLineEdit *portEdit = new QLineEdit(QString::number(ethernet->get_port()));
    portEdit->setReadOnly(true);
    mainGridLayout->addWidget(portLabel, 0, 2);
    mainGridLayout->addWidget(portEdit, 1, 2);

    /* Received Bytes */
    QLabel *byteLabel = new QLabel("Bytes");
    QLineEdit *byteEdit = new QLineEdit("0");
    byteEdit->setDisabled(true);
    connect(ethernet, SIGNAL(bytes_changed(const QString &)), byteEdit, SLOT(setText(const QString &)));
    mainGridLayout->addWidget(byteLabel, 0, 3);
    mainGridLayout->addWidget(byteEdit, 1, 3);

    /* Received Files */
    QLabel *fileLabel = new QLabel("Files");
    QLineEdit *fileEdit = new QLineEdit("0");
    fileEdit->setDisabled(true);
    connect(ethernet, SIGNAL(files_changed(const QString &)), fileEdit, SLOT(setText(const QString &)));
    mainGridLayout->addWidget(fileLabel, 0, 4);
    mainGridLayout->addWidget(fileEdit, 1, 4);

    /* Activity Indicator */
//    QLabel *activeLabel = new QLabel("Active");
//    QLabel *placeHolder = new QLabel("<QIcon>");
    QLabel *activeLabel = new QLabel("Active");
    IndicatorIcon *activeIndicator = new IndicatorIcon("<Icon>", QPixmap(":/icon/ETH_connected.png"), QPixmap(":/icon/ETH_disconnected.png"), QPixmap(":/icon/ETH_event.png"));
    connect(ethernet, SIGNAL(status_changed(int)), activeIndicator, SLOT(set_connected(int)));
    connect(ethernet, SIGNAL(files_changed(const QString &)), activeIndicator, SLOT(set_event()));
    mainGridLayout->addWidget(activeLabel, 0, 5);
    mainGridLayout->addWidget(activeIndicator, 1, 5);

    //Use void connection_timed_out() for signal activation

    setLayout(mainGridLayout);
}
