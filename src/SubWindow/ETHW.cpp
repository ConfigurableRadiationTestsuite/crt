#include "ETHW.h"

#include "src/Components/Ethernet.h"
#include "src/Components/IndicatorIcon.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

ETHW::ETHW(RunManager *m_runManager, Ethernet *ethernet)
    : SubWindow(m_runManager, ethernet), ethernet(ethernet) {

    create_layout();
}

ETHW::~ETHW() {
    delete ethernet;
}

void ETHW::create_layout() {
    QGridLayout *mainGridLayout = new QGridLayout;
    int pos = 0;

    /* Signal button */
    QPushButton *signalButton = new QPushButton;
    signalButton->setText("Add Signal");
    connect(signalButton, SIGNAL(clicked()), this, SLOT(show_signal_dialog()));
    connect(ethernet, SIGNAL(connection_timed_out()), this, SLOT(trigger_signal_list()));
    mainGridLayout->addWidget(signalButton, 1, pos++);

    /* Name */
    QLabel *nameLabel = new QLabel("Name");
    QLineEdit *nameEdit = new QLineEdit(ethernet->get_element_name());
    nameEdit->setReadOnly(true);
    mainGridLayout->addWidget(nameLabel, 0, pos);
    mainGridLayout->addWidget(nameEdit, 1, pos++);

    /* Address */
    QLabel *addressLabel = new QLabel("Address");
    QLineEdit *addressEdit = new QLineEdit;
    addressEdit->setReadOnly(true);
    mainGridLayout->addWidget(addressLabel, 0, pos);
    mainGridLayout->addWidget(addressEdit, 1, pos++);

    /* Port */
    QLabel *portLabel = new QLabel("Port");
    QLineEdit *portEdit = new QLineEdit(QString::number(ethernet->get_port()));
    portEdit->setReadOnly(true);
    mainGridLayout->addWidget(portLabel, 0, pos);
    mainGridLayout->addWidget(portEdit, 1, pos++);

    /* Received Bytes */
    QLabel *byteLabel = new QLabel("Bytes");
    QLineEdit *byteEdit = new QLineEdit("0");
    byteEdit->setDisabled(true);
    connect(ethernet, SIGNAL(bytes_changed(const QString &)), byteEdit, SLOT(setText(const QString &)));
    mainGridLayout->addWidget(byteLabel, 0, pos);
    mainGridLayout->addWidget(byteEdit, 1, pos++);

    /* Received Files */
    QLabel *fileLabel = new QLabel("Files");
    QLineEdit *fileEdit = new QLineEdit("0");
    fileEdit->setDisabled(true);
    connect(ethernet, SIGNAL(files_changed(const QString &)), fileEdit, SLOT(setText(const QString &)));
    mainGridLayout->addWidget(fileLabel, 0, pos);
    mainGridLayout->addWidget(fileEdit, 1, pos++);

    /* Activity Indicator */
    QLabel *activeLabel = new QLabel("Active");
    IndicatorIcon *activeIndicator = new IndicatorIcon("<Icon>", QPixmap(":/icon/ETH_connected.png"), QPixmap(":/icon/ETH_disconnected.png"), QPixmap(":/icon/ETH_waiting.png"), QPixmap(":/icon/ETH_event.png"));
    connect(ethernet, SIGNAL(status_changed(int)), activeIndicator, SLOT(set_status(int)));
    connect(ethernet, SIGNAL(files_changed(const QString &)), activeIndicator, SLOT(set_event()));
    mainGridLayout->addWidget(activeLabel, 0, pos);
    mainGridLayout->addWidget(activeIndicator, 1, pos++);

    setLayout(mainGridLayout);
}
