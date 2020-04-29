#include "PROGW.h"

#include "src/Components/ProgrammStarter.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

PROGW::PROGW(ProgrammStarter *programmStarter, EventManager *m_eventManager)
    : SubWindow(m_eventManager), programmStarter(programmStarter) {

}

PROGW::~PROGW() {}

void PROGW::create_layout() {
    mainVLayout = new QVBoxLayout(this);

    QHBoxLayout *optionLayout = new QHBoxLayout;

    /* Early Logging */
    QCheckBox * earlyLogging = new QCheckBox("Early Logging");
    optionLayout->addWidget(earlyLogging);

    /* Trigger */
    QCheckBox *trigger = new QCheckBox("Trigger");
    optionLayout->addWidget(trigger);

    /* Start */
    QPushButton *startButton = new QPushButton("Start");
    optionLayout->addWidget(startButton);

    QHBoxLayout *pathLayout = new QHBoxLayout;

    /* Programmpath */
    QLineEdit *programmPath = new QLineEdit;
    pathLayout->addWidget(programmPath);

    QPushButton *setPath = new QPushButton("New Path");
    pathLayout->addWidget(setPath);

    /* Console */


    mainVLayout->addLayout(optionLayout);
    mainVLayout->addLayout(pathLayout);
    //add Console

    setLayout(mainVLayout);
}
