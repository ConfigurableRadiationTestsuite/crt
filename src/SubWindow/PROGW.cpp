#include "PROGW.h"

#include "src/Components/ProgrammStarter.h"

PROGW::PROGW(ProgrammStarter *programmStarter, EventManager *m_eventManager)
    : SubWindow(m_eventManager), programmStarter(programmStarter) {

}

PROGW::~PROGW() {}

void PROGW::create_layout() {

}
