#include "FFOXW.h"

#include "src/Components/Fieldfox.h"
#include "src/Plot/FFOXPlot.h"

#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>

FFOXW::FFOXW(RunManager *m_runManager, Fieldfox *fox)
    : SubWindow(m_runManager, fox), fox(fox) {}

FFOXW::~FFOXW() {
    delete fox;
}

void FFOXW::create_layout() {
    QVBoxLayout *mainVLayout = new QVBoxLayout;

    QHBoxLayout *topLineLayout = new QHBoxLayout;
    QGroupBox *settingsBox = new QGroupBox("Settings");

    /* Start frequency */
    QLineEdit *startFreqLine = new QLineEdit;
    startFreqLine->setText(QString::number(fox->get_start_freq()));
    startFreqLine->setDisabled(true);
    topLineLayout->addWidget(new QLabel("Start Frequency"));
    topLineLayout->addWidget(startFreqLine);

    /* Stop frequency */
    QLineEdit *stopFreqLine = new QLineEdit;
    stopFreqLine->setText(QString::number(fox->get_stop_freq()));
    stopFreqLine->setDisabled(true);
    topLineLayout->addWidget(new QLabel("Stop Frequency"));
    topLineLayout->addWidget(stopFreqLine);

    /* Points */
    QLineEdit *pointLine = new QLineEdit;
    pointLine->setText(QString::number(fox->get_points()));
    pointLine->setDisabled(true);
    topLineLayout->addWidget(new QLabel("Points"));
    topLineLayout->addWidget(pointLine);

    /* Graph */
    QVBoxLayout *graphLayout = new QVBoxLayout;
    QGroupBox *graphBox = new QGroupBox("Plot");
    QCustomPlot *plot = new QCustomPlot(this);
    plot->setGeometry(QRect());
    plot->setMinimumHeight(256);
    foxplot = new FFOXPlot(plot, fox->get_points());
    graphLayout->addWidget(plot);
    graphBox->setLayout(graphLayout);
    connect(fox, SIGNAL(data_available(const QVector<double> &)), foxplot, SLOT(update_data(const QVector<double> &)));

    /* Assemble */
    settingsBox->setLayout(topLineLayout);

    mainVLayout->addWidget(settingsBox);
    mainVLayout->addWidget(graphBox);

    setLayout(mainVLayout);

    emit layout_done();
}
