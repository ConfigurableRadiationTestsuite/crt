#include "OSCPlot.h"

#include "src/Components/OSC.h"

OSCPlot::OSCPlot(QCustomPlot *m_plot, OSC * osc)
    : Plot(m_plot, 256, 30), osc(osc) {

    recreate_axis(i_axis);
    recreate_axis(q_axis);

    layoutUpdateTimer = new QTimer;
    connect(layoutUpdateTimer, SIGNAL(timeout()), this, SLOT(update_layout()));
    layoutUpdateTimer->start(1000);

    plotUpdateTimer = new QTimer;
    connect(plotUpdateTimer, SIGNAL(timeout()), this, SLOT(update_plot()));
    plotUpdateTimer->start(200);

    create_layout();
}

OSCPlot::~OSCPlot() {
    delete layoutUpdateTimer;
    delete plotUpdateTimer;
}
