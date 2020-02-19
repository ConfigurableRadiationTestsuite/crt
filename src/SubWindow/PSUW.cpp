#include "PSUW.h"

#include "src/Components/PSU.h"
#include "src/Plot/PSUPlot.h"
#include "src/Plot/qcustomplot.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

PSUW::PSUW(PSU *psu, EventManager *m_eventManager) : SubWindow(m_eventManager), psu(psu) {
    cfg_element = psu;

    connect(this, SIGNAL(signal_on()), psu, SLOT(switch_on()));
    connect(this, SIGNAL(signal_off()), psu, SLOT(switch_off()));
    connect(this, SIGNAL(signal_start_log()), psu, SLOT(start_logging()));
    connect(this, SIGNAL(signal_stop_log()), psu, SLOT(stop_logging()));

    eventManager->add_signal(psu->get_element_name() + " Start Log", SignalType::start_log, this, &SubWindow::signal_start_log);
    eventManager->add_signal(psu->get_element_name() + " Stop Log", SignalType::stop_log, this, &SubWindow::signal_stop_log);
    eventManager->add_signal(psu->get_element_name() + " Switch On", SignalType::on, this, &SubWindow::signal_on);
    eventManager->add_signal(psu->get_element_name() + " Switch Off", SignalType::off, this, &SubWindow::signal_off);

    create_layout();
}

PSUW::~PSUW() {
    //Degregister signals
    eventManager->delete_signal(&SubWindow::signal_on);
    eventManager->delete_signal(&SubWindow::signal_off);
    eventManager->delete_signal(&SubWindow::signal_start_log);
    eventManager->delete_signal(&SubWindow::signal_stop_log);

    delete psu;
    delete mainHLayout;
}

void PSUW::create_layout() {
    mainHLayout = new QHBoxLayout;

    PSUChannel * channel;
    foreach (channel, psu->get_channel_list()) {
        QGroupBox * channelGroupBox = new QGroupBox("Channel: " + QString::number(channel->get_number()));
        QGridLayout *psuGridLayout = new QGridLayout;

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
        QCheckBox * enableBox = new QCheckBox("Enable", this);
        psuGridLayout->addWidget(enableBox, 0, 2);
        connect(enableBox, SIGNAL(stateChanged(int)), channel, SLOT(set_enable(int)));
        connect(channel, SIGNAL(enable_changed(bool)), enableBox, SLOT(setChecked(bool)));

        /* Trigger */
        QCheckBox * triggerBox = new QCheckBox("Trigger", this);
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