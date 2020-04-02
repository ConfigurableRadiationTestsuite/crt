#include "RunManager.h"

#include "EventManager.h"

#include <QElapsedTimer>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>

RunManager::RunManager(EventManager *eventManager) : eventManager(eventManager) {
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

    /* Color palette */
    QPalette *grey = new QPalette();
    grey->setColor(QPalette::Base,Qt::lightGray);
    grey->setColor(QPalette::Text,Qt::black);

    /* Button to create a new run */
    QPushButton *runButton = new QPushButton;
    runButton->setText("New run");
    connect(runButton, SIGNAL(clicked()), this, SLOT(create_new_run()));
    connect(this, SIGNAL(enable_run_button(bool)), runButton, SLOT(setEnabled(bool)));

    /* Field for run folder */
    QLineEdit *runNameLine = new QLineEdit;
    runNameLine->setText("<NAME>");
    runNameLine->setReadOnly(true);
    runNameLine->setPalette(*grey);
    connect(this, SIGNAL(run_name_changed(const QString &)), runNameLine, SLOT(setText(const QString &)));

    /* Current run time */
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

    /* Create new run log for start / stop / various */
    register_component(this, "RunManager");
    set_file_header(this, {"Action"});
    set_run_mode(RunMode::Creation);

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
    if(isRunning)
        return;

    emit enable_run_button(false);

    runTime->start();
    isRunning = true;

    eventManager->trigger_start_log();
    eventManager->trigger_on();

    set_run_mode(RunMode::Start);
}

void RunManager::stop_run() {
    if(!isRunning)
        return;

    offsetTime += runTime->isValid() ? runTime->elapsed() / 1000 : 0;
    runTime->invalidate();
    isRunning = false;

    eventManager->trigger_off();
    eventManager->trigger_stop_log();

    set_run_mode(RunMode::Stop);

    emit enable_run_button(true);
}

void RunManager::set_run_mode(enum RunMode mode) {
    append_value_to_file(this, double(mode));
    emit run_mode_changed(mode);
}
