#include "OSCW.h"

#include "src/Components/OSC.h"
#include "src/Plot/OSCPlot.h"

OSCW::OSCW(RunManager *m_runManager, OSC *osc)
    : SubWindow(m_runManager, osc), osc(osc) {

    /* Connect and register signals */
}

OSCW::~OSCW() {
    /* Degregister signals */

    delete osc;
}

void OSCW::create_layout() {
    emit layout_done();
}
