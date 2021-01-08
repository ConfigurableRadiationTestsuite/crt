#include "OSCW.h"

#include "src/Components/OSC.h"
#include "src/Plot/OSCPlot.h"

OSCW::OSCW(RunManager *m_runManager, OSC *osc)
    : SubWindow(m_runManager, osc), osc(osc) {

    /* Connect and register signals */
    //connect start
    //connect stop
}

OSCW::~OSCW() {
    /* Degregister signals */

    delete osc;
}

void OSCW::create_layout() {
    /* main layout */
    //Mode: Start, Stop, Clear (Start/Stop Button, Clear Button)

    //Trigger: Auto, Normal, Single (Radio Button)
    //Trigger: Channel (Dropdown)

    //Timebase (Dropdown)

    /* channels */
        //Set: Enabled (Checkbox)

        //Get: VMax, VMin, Vpp, Hz (Disabled QLineEdit)

        //Set: Range, Offset (QLineEdit)

    /* Plot */
    //plot->add(OSCChannel);
    //connect(osc->range, plot->range);

    emit layout_done();
}
