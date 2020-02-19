#include "RunManager.h"

#include <QElapsedTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QFileDialog>

RunManager::RunManager() {
    runTime = new QElapsedTimer;
    offsetTime = 0;

    updateTime = new QTimer;
    updateTime->start(1000);
    connect(updateTime, SIGNAL(timeout()), this, SLOT(update_run()));

    create_layout();
}

RunManager::~RunManager() {}

void RunManager::create_layout() {
    QHBoxLayout *mainLayout = new QHBoxLayout;

    QPalette *grey = new QPalette();
    grey->setColor(QPalette::Base,Qt::lightGray);
    grey->setColor(QPalette::Text,Qt::black);

    QPushButton *runButton = new QPushButton;
    runButton->setText("New run");
    connect(runButton, SIGNAL(clicked()), this, SLOT(create_new_run()));
    connect(this, SIGNAL(enable_run_button(bool)), runButton, SLOT(setEnabled(bool)));

    QLineEdit *runNameLine = new QLineEdit;
    runNameLine->setText("<NAME>");
    runNameLine->setReadOnly(true);
    runNameLine->setPalette(*grey);
    connect(this, SIGNAL(run_name_changed(const QString &)), runNameLine, SLOT(setText(const QString &)));

    QLineEdit *runTimeLine = new QLineEdit;
    runTimeLine->setText("000:00:00");
    runTimeLine->setReadOnly(true);
    runTimeLine->setPalette(*grey);
    connect(this, SIGNAL(run_time_changed(const QString &)), runTimeLine, SLOT(setText(const QString &)));

    mainLayout->addWidget(runButton);
    mainLayout->addWidget(runNameLine);
    mainLayout->addWidget(runTimeLine);

    setLayout(mainLayout);
}

void RunManager::create_new_run() {
    QString folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./", QFileDialog::ShowDirsOnly);

    if(QFileInfo::exists(folder))
        set_root_directory(folder);
    else
        return ;

    //Create new run log for start / stop / various
    register_component(this, "RunManager");
    set_file_header(this, {"Action"});
    append_value_to_file(this, double(RunMode::Creation));

    offsetTime = 0;
    emit run_name_changed(folder);
}

void RunManager::update_run() {
    long long time = runTime->isValid() ? runTime->elapsed() / 1000 : 0;
    time += offsetTime;
    uint hour, min, sec;

    sec = time%60;
    time = (time - time%60) / 60;
    min = time%60;
    time = (time - time%60) / 60;
    hour = uint(time);

    QString text = QString("%1:%2:%3").arg(hour, 3, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    emit run_time_changed(text);
}

void RunManager::start_run() {
    runTime->start();

    append_value_to_file(this, double(RunMode::Start));
    emit enable_run_button(false);
}

void RunManager::stop_run() {
    offsetTime += runTime->isValid() ? runTime->elapsed() / 1000 : 0;
    runTime->invalidate();

    append_value_to_file(this, double(RunMode::Stop));
    emit enable_run_button(true);
}
