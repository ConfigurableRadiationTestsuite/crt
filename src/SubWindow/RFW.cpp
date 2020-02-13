#include "RFW.h"

#include "src/Plot/qcustomplot.h"
#include "src/Components/RFIO.h"
#include "src/Components/RFIOChannel.h"
#include "src/Plot/RFPlot.h"

#include <QHBoxLayout>

RFW::RFW(RFIO *rfio, EventManager *m_eventManager) : SubWindow(m_eventManager), rfio(rfio) {
    cfg_element = rfio;

    create_layout();
}

RFW::~RFW() {}

void RFW::create_layout() {
    subHorizontalLayout = new QHBoxLayout;

    RFIOChannel * channel;
    foreach (channel, rfio->get_channel_list()) {
        QGroupBox * channelGroupBox = new QGroupBox("Channel: " + QString::number(channel->get_number()));
        QHBoxLayout *channelHLayout = new QHBoxLayout;

        /* Plot */
        QCustomPlot *plot = new QCustomPlot(this);
        RFPlot *psuplot = new RFPlot(plot);
        plot->setGeometry(QRect());
        plot->setMinimumHeight(128);
        plot->setMaximumHeight(512);
        connect(this, SIGNAL(destroyed()), psuplot, SLOT(deleteLater()));
        channelHLayout->addWidget(plot);

        QVBoxLayout *evalLayout = new QVBoxLayout;

        /* Evaluate */
        QCheckBox * evalBox = new QCheckBox("Evaluate", this);
        evalBox->setDisabled(true);
        evalLayout->addWidget(evalBox);
        connect(evalBox, SIGNAL(stateChanged(int)), channel, SLOT(set_evaluation(int)));
        connect(channel, SIGNAL(evaluable_changed(bool)), evalBox, SLOT(setChecked(bool)));
        connect(channel, SIGNAL(evaluable_changed(bool)), evalBox, SLOT(setDisabled(bool)));

        /* Slider */
        QSlider *precisionSlide = new QSlider(Qt::Vertical);
        precisionSlide->setRange(1, 10);
        precisionSlide->setTickPosition(QSlider::TicksRight);
        precisionSlide->setValue(1);
        connect(precisionSlide, SIGNAL(valueChanged(int)), channel, SLOT(set_margin(int)));
        evalLayout->addWidget(new QLabel("Margin"));
        evalLayout->addWidget(precisionSlide);

        channelHLayout->addLayout(evalLayout);

        channelGroupBox->setLayout(channelHLayout);
        subHorizontalLayout->addWidget(channelGroupBox);
    }

    setLayout(subHorizontalLayout);
}
