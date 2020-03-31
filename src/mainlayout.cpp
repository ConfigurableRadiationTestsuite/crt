#include "mainlayout.h"

#include "src/Configuration/ConfigManager.h"
#include "src/Manager/EventManager.h"
#include "src/Manager/RunManager.h"
#include "Tabs/PSUTab.h"
#include "Tabs/LBJTab.h"
#include "Tabs/RFTab.h"

#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QToolButton>

MainLayout::MainLayout() {
    configManager = new ConfigManager;
    eventManager = new EventManager;
    runManager = new RunManager;

    create_layout();
}

MainLayout::~MainLayout() {}

void MainLayout::create_layout() {
    mainVerticalLayout = new QVBoxLayout();

    mainVerticalLayout->setSpacing(20);

    //On top create a toolbar with save/load/start/stop buttons
    mainVerticalLayout->setMenuBar(create_toolbar());

    //TODO Display run options
    mainVerticalLayout->addWidget(runManager);

    //Below create the tabs for psu/eth/labjack/...
    mainVerticalLayout->addWidget(create_window_tabs());

    setLayout(mainVerticalLayout);
}

QToolBar * MainLayout::create_toolbar() {
    toolbar = new QToolBar;

    //Save config button
    QAction* save_cfg = toolbar->addAction("Save config");
    connect(save_cfg, SIGNAL(triggered()), configManager, SLOT(save_config()));

    //Load config button
    QAction* load_cfg = toolbar->addAction("Load config");
    connect(load_cfg, SIGNAL(triggered()), configManager, SLOT(load_config()));

    toolbar->addSeparator();

    //Start/Stop button
    startTestButton = toolbar->addAction("Start Test");
    startTestButton->setIcon(QIcon("../CRT/icon/startButton.png"));
    startTestButton->setCheckable(true);
    connect(startTestButton, SIGNAL(triggered()), this, SLOT(start_test()));

    stopTestButton = toolbar->addAction("Stop Test");
    stopTestButton->setIcon(QIcon("../CRT/icon/stopButton.png"));
    connect(stopTestButton, SIGNAL(triggered()), this, SLOT(stop_test()));

    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setIconSize(QSize(16, 16));

    return toolbar;
}

QTabWidget * MainLayout::create_window_tabs() {
    windowTabs= new QTabWidget;

    //Power supplies
    windowTabs->addTab(new PSUTab(configManager, eventManager, runManager), "PSU");

    //Labjack
    windowTabs->addTab(new LBJTab(configManager, eventManager, runManager), "Labjack");

    //RF
    //windowTabs->addTab(new RFTab(configManager, eventManager, runManager), "RF");

    return windowTabs;
}

void MainLayout::start_test() {
    if(runManager->is_running())
        return;

    eventManager->trigger_on();
    eventManager->trigger_start_log();

    startTestButton->setIcon(QIcon("../CRT/icon/startButton_active.png"));
    stopTestButton->setIcon(QIcon("../CRT/icon/stopButton.png"));

    startTestButton->setChecked(true);

    runManager->start_run();
}

void MainLayout::stop_test() {
    runManager->stop_run();

    eventManager->trigger_off();
    eventManager->trigger_stop_log();

    startTestButton->setChecked(false);

    startTestButton->setIcon(QIcon("../CRT/icon/startButton.png"));
    stopTestButton->setIcon(QIcon("../CRT/icon/stopButton_active.png"));
}
