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
    QAction* start_test = toolbar->addAction("Start Test");
    start_test->setIcon(QIcon("..//configurable-radiation-testsuite/icon/startButton.png"));
    start_test->setCheckable(true);
    connect(start_test, SIGNAL(triggered()), eventManager, SLOT(trigger_on()));
    connect(start_test, SIGNAL(triggered()), eventManager, SLOT(trigger_start_log()));
    connect(start_test, SIGNAL(triggered(bool)), start_test, SLOT(setDisabled(bool)));
    //Connect the runManager
    connect(start_test, SIGNAL(triggered()), runManager, SLOT(start_run()));

    QAction* stop_test = toolbar->addAction("Stop Test");
    stop_test->setIcon(QIcon("..//configurable-radiation-testsuite/icon/stopButton.png"));
    connect(stop_test, SIGNAL(triggered()), eventManager, SLOT(trigger_off()));
    connect(stop_test, SIGNAL(triggered()), eventManager, SLOT(trigger_stop_log()));
    connect(stop_test, SIGNAL(triggered(bool)), start_test, SLOT(setDisabled(bool)));
    connect(stop_test, SIGNAL(triggered(bool)), start_test, SLOT(setChecked(bool)));
    //Connect the runManager
    connect(stop_test, SIGNAL(triggered()), runManager, SLOT(stop_run()));

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

    return windowTabs;
}
