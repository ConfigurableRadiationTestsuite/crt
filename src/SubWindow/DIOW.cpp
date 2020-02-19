#include "DIOW.h"

#include "src/Components/DIOM.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

DIOW::DIOW(DIOM *diom, EventManager *m_eventManager) : SubWindow(m_eventManager), diom(diom) {
    cfg_element = diom;

    create_layout();
}

DIOW::~DIOW() {
    delete diom;
    delete mainHLayout;
}

void DIOW::create_layout() {
    mainHLayout = new QHBoxLayout;


    setLayout(mainHLayout);
}
