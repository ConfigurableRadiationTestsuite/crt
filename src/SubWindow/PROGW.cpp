#include "PROGW.h"

#include "src/Components/ProgrammStarter.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

PROGW::PROGW(ProgrammStarter *programmStarter, EventManager *m_eventManager)
    : SubWindow(m_eventManager), programmStarter(programmStarter) {
    cfg_element = programmStarter;

    connect(this, SIGNAL(signal_on()), programmStarter, SLOT(execute_programm()));
    connect(this, SIGNAL(signal_off()), programmStarter, SLOT(kill_programm()));
    connect(this, SIGNAL(signal_start_log()), programmStarter, SLOT(start_logging()));
    connect(this, SIGNAL(signal_stop_log()), programmStarter, SLOT(stop_logging()));

    eventManager->add_signal(programmStarter->get_element_name() + " Start Log", SignalType::start_log, this, &SubWindow::signal_start_log);
    eventManager->add_signal(programmStarter->get_element_name() + " Stop Log", SignalType::stop_log, this, &SubWindow::signal_stop_log);
    eventManager->add_signal(programmStarter->get_element_name() + " Execute", SignalType::on, this, &SubWindow::signal_on);
    eventManager->add_signal(programmStarter->get_element_name() + " Kill", SignalType::off, this, &SubWindow::signal_off);

    create_layout();
}

PROGW::~PROGW() {
    eventManager->delete_signal(&SubWindow::signal_on);
    eventManager->delete_signal(&SubWindow::signal_off);
    eventManager->delete_signal(&SubWindow::signal_start_log);
    eventManager->delete_signal(&SubWindow::signal_stop_log);

    delete programmStarter;
    delete mainVLayout;
}

void PROGW::create_layout() {
    mainVLayout = new QVBoxLayout(this);

    QHBoxLayout *optionLayout = new QHBoxLayout;

    /* Early Logging */
    QCheckBox * earlyLogging = new QCheckBox("Early Logging");
    earlyLogging->setDisabled(true);
    optionLayout->addWidget(earlyLogging);
    connect(earlyLogging, SIGNAL(stateChanged(int)), programmStarter, SLOT(set_early_logging(int)));
    connect(eventManager, SIGNAL(logging_disabled(bool)), earlyLogging, SLOT(setDisabled(bool)));

    /* Trigger */
    QCheckBox *trigger = new QCheckBox("Trigger");
    optionLayout->addWidget(trigger);
    connect(trigger, SIGNAL(stateChanged(int)), programmStarter, SLOT(set_trigger(int)));
    connect(programmStarter, SIGNAL(announce_run(bool)), trigger, SLOT(setDisabled(bool)));

    /* Stop */
    QPushButton *startButton = new QPushButton("Start");
    optionLayout->addWidget(startButton);
    connect(startButton, SIGNAL(clicked()), programmStarter, SLOT(execute_programm()));
    connect(programmStarter, SIGNAL(announce_trigger(bool)), startButton, SLOT(setDisabled(bool)));

    QPushButton *stopButton = new QPushButton("Stop");
    optionLayout->addWidget(stopButton);
    connect(stopButton, SIGNAL(clicked()), programmStarter, SLOT(kill_programm()));
    connect(programmStarter, SIGNAL(announce_trigger(bool)), stopButton, SLOT(setDisabled(bool)));

    /* Spacer */
    QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    optionLayout->addSpacerItem(space);

    /* Second Row */

    QHBoxLayout *pathLayout = new QHBoxLayout;

    /* Programmpath */
    QLineEdit *programmPath = new QLineEdit;
    programmPath->setDisabled(true);
    connect(programmStarter, SIGNAL(announce_path(const QString &)), programmPath, SLOT(setText(const QString &)));
    pathLayout->addWidget(programmPath);

    QPushButton *setPath = new QPushButton("New Path");
    connect(setPath, SIGNAL(clicked()), this, SLOT(path_dialog()));
    connect(programmStarter, SIGNAL(announce_run(bool)), setPath, SLOT(setDisabled(bool)));
    pathLayout->addWidget(setPath);

    /* Console */
    QTextEdit * consoleText = new QTextEdit;
    connect(programmStarter, SIGNAL(data_available(const QString &)), consoleText, SLOT(append(const QString &)));

    mainVLayout->addLayout(optionLayout);
    mainVLayout->addLayout(pathLayout);
    mainVLayout->addWidget(consoleText);

    setLayout(mainVLayout);
}

void PROGW::path_dialog() {
    programmStarter->set_path(QFileDialog::getOpenFileName(this, tr("Open File"), "./"));
}
