#include "PROGW.h"

#include "src/Components/ProgrammStarter.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

PROGW::PROGW(ProgrammStarter *programmStarter, EventManager *m_eventManager)
    : SubWindow(m_eventManager), programmStarter(programmStarter) {
    cfg_element = programmStarter;

    create_layout();
}

PROGW::~PROGW() {}

void PROGW::create_layout() {
    mainVLayout = new QVBoxLayout(this);

    QHBoxLayout *optionLayout = new QHBoxLayout;

    /* Early Logging */
    QCheckBox * earlyLogging = new QCheckBox("Early Logging");
    optionLayout->addWidget(earlyLogging);
    connect(earlyLogging, SIGNAL(stateChanged(int)), programmStarter, SLOT(set_early_logging(int)));

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
    pathLayout->addWidget(programmPath);

    QPushButton *setPath = new QPushButton("New Path");
    pathLayout->addWidget(setPath);

    /* Console */
    QTextEdit * consoleText = new QTextEdit;
    connect(programmStarter, SIGNAL(data_available(const QString &)), consoleText, SLOT(append(const QString &)));

    mainVLayout->addLayout(optionLayout);
    mainVLayout->addLayout(pathLayout);
    mainVLayout->addWidget(consoleText);

    setLayout(mainVLayout);
}
