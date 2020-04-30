#include "mainlayout.h"

#include "src/Configuration/ConfigManager.h"
#include "src/Manager/EventManager.h"
#include "src/Manager/RunManager.h"
#include "Tabs/LBJTab.h"
#include "Tabs/PROGTab.h"
#include "Tabs/PSUTab.h"
#include "Tabs/RFTab.h"

#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

MainLayout::MainLayout() {
    configManager = new ConfigManager;
    eventManager = new EventManager;
    runManager = new RunManager(eventManager);

    create_layout();
}

MainLayout::~MainLayout() {}

void MainLayout::create_layout() {
    mainVerticalLayout = new QVBoxLayout(this);

    mainVerticalLayout->setSpacing(20);

    //On top create a toolbar with save/load/start/stop buttons
    mainVerticalLayout->setMenuBar(create_toolbar());

    //Display run options
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
    startTestButton->setIcon(QIcon(":/icon/startButton.png"));
    startTestButton->setDisabled(true);
    connect(startTestButton, SIGNAL(triggered()), runManager, SLOT(start_run()));
    connect(runManager, SIGNAL(run_mode_changed(enum RunMode)), this, SLOT(set_start_button(enum RunMode)));
    connect(eventManager, SIGNAL(logging_disabled(bool)), startTestButton, SLOT(setDisabled(bool)));

    stopTestButton = toolbar->addAction("Stop Test");
    stopTestButton->setIcon(QIcon(":/icon/stopButton.png"));
    connect(stopTestButton, SIGNAL(triggered()), runManager, SLOT(stop_run()));
    connect(runManager, SIGNAL(run_mode_changed(enum RunMode)), this, SLOT(set_stop_button(enum RunMode)));

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
    windowTabs->addTab(new RFTab(configManager, eventManager, runManager), "RF");

    //PROG
    windowTabs->addTab(new PROGTab(configManager, eventManager, runManager), "PROG");

    return windowTabs;
}

void MainLayout::set_start_button(enum RunMode mode) {
    switch(mode) {
    case Creation:
        startTestButton->setIcon(QIcon(":icon/startButton.png"));
        break;
    case Start:
        startTestButton->setIcon(QIcon(":icon/startButton_active.png"));
        break;
    case Stop:
        startTestButton->setIcon(QIcon(":icon/startButton.png"));
        break;
    default:
        break;
    }
}

void MainLayout::set_stop_button(enum RunMode mode) {
    switch(mode) {
    case Creation:
        stopTestButton->setIcon(QIcon(":icon/stopButton.png"));
        break;
    case Start:
        stopTestButton->setIcon(QIcon(":icon/stopButton.png"));
        break;
    case Stop:
        stopTestButton->setIcon(QIcon(":icon/stopButton_active.png"));
        break;
    default:
        break;
    }
}
