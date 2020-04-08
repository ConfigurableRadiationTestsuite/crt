#include "RFW.h"

#include "src/Plot/qcustomplot.h"
#include "src/Components/RFIO.h"
#include "src/Components/RFIOChannel.h"
#include "src/Plot/RFPlot.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

RFW::RFW(RFIO *rfio, EventManager *m_eventManager) : SubWindow(m_eventManager), rfio(rfio) {
    cfg_element = rfio;

    connect(this, SIGNAL(signal_start_log()), rfio, SLOT(start_logging()));
    connect(this, SIGNAL(signal_stop_log()), rfio, SLOT(stop_logging()));

    eventManager->add_signal(rfio->get_element_name() + " Start Log", SignalType::start_log, this, &SubWindow::signal_start_log);
    eventManager->add_signal(rfio->get_element_name() + " Stop Log", SignalType::stop_log, this, &SubWindow::signal_stop_log);

    create_layout();
}

RFW::~RFW() {
    //Deregister signals
    eventManager->delete_signal(&SubWindow::signal_start_log);
    eventManager->delete_signal(&SubWindow::signal_stop_log);

    delete rfio;
}

void RFW::create_layout() {
    mainVLayout = new QVBoxLayout(this);

    /* Header */
    headerHLayout = new QHBoxLayout;

    /* Signal button */
    QPushButton *signalButton = new QPushButton;
    signalButton->setText("Add Signal");
    connect(signalButton, SIGNAL(clicked()), this, SLOT(show_signal_dialog()));
    headerHLayout->addWidget(signalButton);

    /* Channel */
    subHLayout = new QHBoxLayout(this);

    RFIOChannel * channel;
    foreach (channel, rfio->get_channel_list()) {
        QGroupBox * channelGroupBox = new QGroupBox("Channel: " + QString::number(channel->get_channel_number()));
        QHBoxLayout *channelHLayout = new QHBoxLayout;

        /* Plot */
        QCustomPlot *plot = new QCustomPlot(this);
        RFPlot *rfplot = new RFPlot(plot, channel);
        plot->setGeometry(QRect());
        plot->setMinimumHeight(128);
        plot->setMinimumWidth(256);
        connect(this, SIGNAL(destroyed()), rfplot, SLOT(deleteLater()));
        channelHLayout->addWidget(plot);

        QVBoxLayout *evalLayout = new QVBoxLayout;

        /* Evaluate */
        QCheckBox * evalBox = new QCheckBox("Evaluate", this);
        evalBox->setDisabled(true);
        evalLayout->addWidget(evalBox);
        connect(channel, SIGNAL(announce_data_valid(bool)), evalBox, SLOT(setDisabled(bool)));
        connect(evalBox, SIGNAL(stateChanged(int)), channel, SLOT(set_data_analyze(int)));

        connect(channel, SIGNAL(error()), this, SLOT(trigger_signal_list()));

        /* Slider */
        QSlider *precisionSlide = new QSlider(Qt::Vertical);
        precisionSlide->setRange(1, 10);
        precisionSlide->setTickPosition(QSlider::TicksRight);
        precisionSlide->setTickInterval(1);
        precisionSlide->setValue(channel->get_margin());
        connect(precisionSlide, SIGNAL(valueChanged(int)), channel, SLOT(set_margin(int)));
        evalLayout->addWidget(new QLabel("Margin"));
        evalLayout->addWidget(precisionSlide);

        channelHLayout->addLayout(evalLayout);

        channelGroupBox->setLayout(channelHLayout);
        subHLayout->addWidget(channelGroupBox);
    }

    mainVLayout->addLayout(headerHLayout);
    mainVLayout->addLayout(subHLayout);

    setLayout(mainVLayout);
}
