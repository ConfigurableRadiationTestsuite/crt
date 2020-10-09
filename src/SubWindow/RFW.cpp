#include "RFW.h"

#include "src/Components/RFIO.h"
#include "src/Plot/RFPlot.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

RFW::RFW(RunManager *m_runManager, RFIO *rfio) : SubWindow(m_runManager, rfio), rfio(rfio) {}

RFW::~RFW() {
    delete rfio;
}

void RFW::create_layout() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /* Header */
    QHBoxLayout *headerHLayout = new QHBoxLayout;

    /* Signal button */
    QPushButton *signalButton = new QPushButton;
    signalButton->setText("Add Signal");
    connect(signalButton, SIGNAL(clicked()), this, SLOT(show_signal_dialog()));
    headerHLayout->addWidget(signalButton);

    /* Seperator */
    QFrame* line_1 = new QFrame();
    line_1->setFrameShape(QFrame::VLine);
    line_1->setFrameShadow(QFrame::Sunken);
    headerHLayout->addWidget(line_1);

    /* Single vs Multishot */
    QButtonGroup * shotGroup = new QButtonGroup;
    QRadioButton * singleShot = new QRadioButton("Single");
    QRadioButton * multiShot = new QRadioButton("Multi");
    shotGroup->addButton(singleShot);
    shotGroup->addButton(multiShot);
    multiShot->setChecked(true);
    rfio->set_multi_shot();
    connect(singleShot, SIGNAL(clicked()), rfio, SLOT(set_single_shot()));
    connect(multiShot, SIGNAL(clicked()), rfio, SLOT(set_multi_shot()));

    headerHLayout->addWidget(singleShot);
    headerHLayout->addWidget(multiShot);

    /* Seperator */
    QFrame* line_2 = new QFrame();
    line_2->setFrameShape(QFrame::VLine);
    line_2->setFrameShadow(QFrame::Sunken);
    headerHLayout->addWidget(line_2);

    /* Resume */
//    QPushButton * resumeButton = new QPushButton;
//    resumeButton->setText("Resume");
//    headerHLayout->addWidget(resumeButton);
//    connect(resumeButton, SIGNAL(clicked()), rfio, SLOT(reconnect()));

//    QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    headerHLayout->addSpacerItem(space);

    /* Channel */
    QHBoxLayout *subHLayout = new QHBoxLayout;

    foreach (RFIOChannel * channel, rfio->get_channel_list()) {
        QGroupBox * channelGroupBox = new QGroupBox("Channel: " + QString::number(channel->get_channel_number()));
        QHBoxLayout *channelHLayout = new QHBoxLayout;

        /* Plot */
        QCustomPlot *plot = new QCustomPlot(this);
        RFPlot *rfplot = new RFPlot(plot, channel);
        plot->setGeometry(QRect());
        plot->setMaximumHeight(512);
        connect(channel, SIGNAL(destroyed()), rfplot, SLOT(deleteLater()));
        connect(channel, SIGNAL(plot_data_changed(const QVector<int> &, const QVector<int> &)), rfplot, SLOT(update_data(const QVector<int> &, const QVector<int>&)));
        channelHLayout->addWidget(plot);
        rfplot->update_layout();

        QVBoxLayout *evalLayout = new QVBoxLayout;

        /* Evaluate */
        QCheckBox * evalBox = new QCheckBox("Evaluate");
        evalBox->setDisabled(true);
        evalLayout->addWidget(evalBox);
        connect(channel, SIGNAL(announce_data_valid(bool)), evalBox, SLOT(setDisabled(bool)));
        connect(evalBox, SIGNAL(stateChanged(int)), channel, SLOT(set_data_analyze(int)));

        connect(channel, SIGNAL(error(QVector<IQSample>, int)), this, SLOT(trigger_signal_list()));

        /* Margin info */
        QLabel *marginLabel = new QLabel("Margin");
        QLineEdit *marginLineEdit = new QLineEdit;
        marginLineEdit->setText("2^0");
        marginLineEdit->setMaxLength(4);
        marginLineEdit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        marginLineEdit->setDisabled(true);
        evalLayout->addWidget(marginLabel);
        evalLayout->addWidget(marginLineEdit);
        connect(channel, SIGNAL(announce_margin_changed(const QString &)), marginLineEdit, SLOT(setText(const QString &)));

        /* Slider */
        QSlider *precisionSlide = new QSlider(Qt::Vertical);
        precisionSlide->setRange(1, 10);
        precisionSlide->setTickPosition(QSlider::TicksRight);
        precisionSlide->setTickInterval(1);
        precisionSlide->setValue(channel->get_margin());
        connect(precisionSlide, SIGNAL(valueChanged(int)), channel, SLOT(set_margin(int)));
        evalLayout->addWidget(precisionSlide);

        channelHLayout->addLayout(evalLayout);

        channelGroupBox->setLayout(channelHLayout);
        subHLayout->addWidget(channelGroupBox);
    }

    mainVLayout->addLayout(headerHLayout);
    mainVLayout->addLayout(subHLayout);

    setLayout(mainVLayout);

    emit layout_done();
}
