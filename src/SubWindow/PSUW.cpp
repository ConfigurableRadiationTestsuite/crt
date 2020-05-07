#include "PSUW.h"

#include "src/Components/PSU.h"
#include "src/Components/Indicator.h"
#include "src/Plot/PSUPlot.h"
#include "src/Plot/qcustomplot.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

PSUW::PSUW(RunManager *m_runManager, PSU *psu) : SubWindow(m_runManager, psu), psu(psu) {
    /* Connect and register signals */
    connect(this, SIGNAL(signal_on()), psu, SLOT(switch_on()));
    eventManager->add_signal(psu->get_element_name() + " Switch On", SignalType::on, this, &SubWindow::signal_on);
    connect(this, SIGNAL(signal_off()), psu, SLOT(switch_off()));
    eventManager->add_signal(psu->get_element_name() + " Switch Off", SignalType::off, this, &SubWindow::signal_off);

    create_layout();
}

PSUW::~PSUW() {
    /* Degregister signals */
    eventManager->delete_signal(&SubWindow::signal_on);
    eventManager->delete_signal(&SubWindow::signal_off);

    delete psu;
    delete mainHLayout;
}

void PSUW::create_layout() {
    mainHLayout = new QHBoxLayout;

    PSUChannel * channel;
    foreach (channel, psu->get_channel_list()) {
        QGroupBox * channelGroupBox = new QGroupBox("Channel: " + QString::number(channel->get_number()));
        QGridLayout *psuGridLayout = new QGridLayout;
        connect(psu, SIGNAL(disconnected(bool)), channelGroupBox, SLOT(setDisabled(bool)));

        /* Voltage */
        QLabel *voltLabel = new QLabel("[V]:");
        QLineEdit *voltLineEdit = new QLineEdit;
        voltLineEdit->setText(QString::number(channel->get_voltage_set()));
        psuGridLayout->addWidget(voltLabel, 0, 0);
        psuGridLayout->addWidget(voltLineEdit, 1, 0);
        connect(voltLineEdit, SIGNAL(textChanged(const QString &)), channel, SLOT(set_voltage(const QString &)));
        connect(channel, SIGNAL(voltage_changed(const QString &)), voltLineEdit, SLOT(setText(const QString &)));

        /* Current */
        QLabel *currentLabel = new QLabel("[mA]:");
        QLineEdit *currentLineEdit = new QLineEdit;
        currentLineEdit->setText(QString::number(channel->get_current_set()));
        psuGridLayout->addWidget(currentLabel, 0, 1);
        psuGridLayout->addWidget(currentLineEdit, 1, 1);
        connect(currentLineEdit, SIGNAL(textChanged(const QString &)), channel, SLOT(set_current(const QString &)));
        connect(channel, SIGNAL(current_changed(const QString &)), currentLineEdit, SLOT(setText(const QString &)));

        /* Enable */
        Indicator * enableBox = new Indicator(QIcon("../CRT/icon/PSU_on.png"), QIcon("../CRT/icon/PSU_off.png"), QIcon("../CRT/icon/PSU_disconnected.png"));
        psuGridLayout->addWidget(enableBox, 0, 2);
        connect(enableBox, SIGNAL(stateChanged(int)), channel, SLOT(set_enable(int)));
        connect(channel, SIGNAL(enable_changed(bool)), enableBox, SLOT(setChecked(bool)));
        connect(psu, SIGNAL(disconnected(bool)), enableBox, SLOT(set_event_icon(bool)));

        /* Trigger */
        QCheckBox * triggerBox = new QCheckBox("Trigger");
        psuGridLayout->addWidget(triggerBox, 1, 2);
        connect(triggerBox, SIGNAL(stateChanged(int)), channel, SLOT(set_trigger(int)));

        /* Plot */
        QCustomPlot *plot = new QCustomPlot(this);
        PSUPlot *psuplot = new PSUPlot(plot, channel, 30, 30);
        plot->setGeometry(QRect());
        plot->setMinimumHeight(128);
        plot->setMaximumHeight(512);
        connect(this, SIGNAL(destroyed()), psuplot, SLOT(deleteLater()));

        QVBoxLayout *psuVLayout = new QVBoxLayout;
        psuVLayout->addLayout(psuGridLayout);
        psuVLayout->addWidget(plot);

        channelGroupBox->setLayout(psuVLayout);
        mainHLayout->addWidget(channelGroupBox);
    }

    /* Master switch */
    if(psu->has_master_switch()) {
        QGridLayout *masterGridLayout = new QGridLayout;
        QGroupBox *masterBoxLayout = new QGroupBox("Master Switch");

        QCheckBox * masterEnable = new QCheckBox("Master", this);
        connect(masterEnable, SIGNAL(stateChanged(int)), psu, SLOT(set_master_enable(int)));
        connect(psu, SIGNAL(master_changed(bool)), masterEnable, SLOT(setChecked(bool)));

        QCheckBox * masterTrigger = new QCheckBox("Trigger", this);
        connect(masterTrigger, SIGNAL(stateChanged(int)), psu, SLOT(set_master_trigger(int)));

        masterGridLayout->addWidget(masterEnable, 0, 0);
        masterGridLayout->addWidget(masterTrigger, 1, 0);

        masterBoxLayout->setLayout(masterGridLayout);
        mainHLayout->addWidget(masterBoxLayout);
    }

    setLayout(mainHLayout);
}
