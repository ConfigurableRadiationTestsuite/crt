#include "DIOW.h"

#include "src/Components/DIOM.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

DIOW::DIOW(DIOM *diom, RunManager *m_runManager) : SubWindow(m_runManager), diom(diom) {
    //component = diom;

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
