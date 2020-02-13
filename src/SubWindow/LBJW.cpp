#include "LBJW.h"

#include "src/Components/Labjack.h"
#include "src/Components/LabjackChannel.h"
#include "src/Plot/qcustomplot.h"
#include "src/Plot/LBJPlot.h"

#include <QVBoxLayout>
#include <QSlider>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QCheckBox>
#include <QGridLayout>

LBJW::LBJW(Labjack *lbj, EventManager *m_eventManager) : SubWindow(m_eventManager), lbj(lbj) {
    cfg_element = lbj;

    connect(this, SIGNAL(signal_start()), lbj, SLOT(start()));
    connect(this, SIGNAL(signal_stop()), lbj, SLOT(stop()));

    eventManager->add_signal(lbj->get_element_name() + " Start", SignalType::start, this, &SubWindow::signal_start);
    eventManager->add_signal(lbj->get_element_name() + " Stop", SignalType::stop, this, &SubWindow::signal_stop);

    create_layout();
}

LBJW::~LBJW() {
    delete lbj;
    delete subVerticalLayout;
}

void LBJW::create_layout() {
    subVerticalLayout = new QVBoxLayout;

    QHBoxLayout * topLineLayout = new QHBoxLayout;

    //Add Trigger button

    //Precision
    QSlider *precisionSlide = new QSlider(Qt::Horizontal);
    precisionSlide->setRange(1, 10);
    precisionSlide->setTickPosition(QSlider::TicksBelow);
    precisionSlide->setValue(1);
    connect(precisionSlide, SIGNAL(valueChanged(int)), lbj, SLOT(set_main_resolution(int)));
    topLineLayout->addWidget(new QLabel("Auflösung"));
    topLineLayout->addWidget(precisionSlide);

    //Add settling
    QLineEdit *settlingLine = new QLineEdit;
    QIntValidator *settlingValid = new QIntValidator(1, 50000, this);
    settlingLine->setText(QString::number(10000));
    settlingLine->setValidator(settlingValid);
    connect(settlingLine, SIGNAL(textChanged(const QString &)), lbj, SLOT(set_main_settling(const QString &)));
    topLineLayout->addWidget(new QLabel("Settling"));
    topLineLayout->addWidget(settlingLine);

    //Add datarate
    QLineEdit *datarateLine = new QLineEdit;
    QIntValidator *datarateValid = new QIntValidator(1, 1000, this);
    datarateLine->setText(QString::number(1));
    datarateLine->setValidator(datarateValid);
    connect(datarateLine, SIGNAL(textChanged(const QString &)), lbj, SLOT(set_samplerate(const QString &)));
    connect(lbj, SIGNAL(samplerate_changed(const QString &)), datarateLine, SLOT(setText(const QString &)));
    topLineLayout->addWidget(new QLabel("Datarate"));
    topLineLayout->addWidget(datarateLine);

    //Maximum Button
    QCheckBox *datarateBox = new QCheckBox;
    connect(datarateBox, SIGNAL(stateChanged(int)), lbj, SLOT(set_maximum_samplerate(int)));
    topLineLayout->addWidget(new QLabel("Maximum"));
    topLineLayout->addWidget(datarateBox);

    //Channel header
    QGridLayout *channelLayout = new QGridLayout;
    channelLayout->addWidget(new QLabel("Channel"), 0, 0);
    channelLayout->addWidget(new QLabel("Boundary"), 0, 1);
    channelLayout->addWidget(new QLabel("Value"), 0, 2);
    channelLayout->addWidget(new QLabel("Gain"), 0, 3);
    channelLayout->addWidget(new QLabel("Graph"), 0, 4);

    //Plot for the channels
    QVBoxLayout *graphLayout = new QVBoxLayout;
    QCustomPlot *plot = new QCustomPlot(this);
    plot->setGeometry(QRect());
    plot->setMinimumHeight(256);
    lbjplot = new LBJPlot(plot);
    graphLayout->addWidget(plot);

    LabjackChannel *channel; int cnt = 1;
    foreach (channel, lbj->get_channel_list()) {
        //Name
        QLabel *channelLabel = new QLabel(channel->get_name());

        //Boundary
        QLineEdit *boundaryLine = new QLineEdit(QString::number(channel->get_boundary()));
        connect(boundaryLine, SIGNAL(textChanged(const QString &)), channel, SLOT(set_boundary(const QString &)));

        //Value
        QLineEdit *valueLine = new QLineEdit(QString::number(channel->get_value()));
        connect(channel, SIGNAL(value_changed(const QString &)), valueLine, SLOT(setText(const QString &)));

        //Gain
        QLineEdit *gainLine = new QLineEdit(QString::number(channel->get_gain()));
        connect(gainLine, SIGNAL(textChanged(const QString &)), channel, SLOT(set_external_gain(const QString &)));

        //Graph
        QCheckBox *graphCheckBox = new QCheckBox;
        graphCheckBox->setStyleSheet("color: " + color_list[cnt%color_list.size()].name + " ;");
        lbjplot->add_channel(channel, color_list[cnt%color_list.size()].color);
        connect(graphCheckBox, SIGNAL(stateChanged(int)), lbjplot->get_plotelement_list().last(), SLOT(set_plot_active(int)));

        channelLayout->addWidget(channelLabel, cnt, 0);
        channelLayout->addWidget(boundaryLine, cnt, 1);
        channelLayout->addWidget(valueLine, cnt, 2);
        channelLayout->addWidget(gainLine, cnt, 3);
        channelLayout->addWidget(graphCheckBox, cnt, 4);
        cnt++;
    }

    subVerticalLayout->addLayout(topLineLayout);
    subVerticalLayout->addLayout(channelLayout);
    subVerticalLayout->addLayout(graphLayout);

    setLayout(subVerticalLayout);
}
