#include "mainlayout.h"

#include "src/Configuration/ConfigManager.h"

#include "detachedwindow.h"
#include "Tabs/ETHTab.h"
#include "Tabs/LBJTab.h"
#include "Tabs/PROGTab.h"
#include "Tabs/PSUTab.h"
#include "Tabs/RFTab.h"
#include "Tabs/SEQTab.h"
#include "Tabs/FFOXTab.h"
#include "Tabs/OSCTab.h"

#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

MainLayout::MainLayout()
{
    configManager = new ConfigManager;
    runManager = new RunManager(new EventManager);

    create_layout();
}

MainLayout::~MainLayout() {}

void MainLayout::create_layout()
{
    mainVerticalLayout = new QVBoxLayout;

    mainVerticalLayout->setSpacing(20);

    //On top create a toolbar with save/load/start/stop buttons
    mainVerticalLayout->setMenuBar(create_toolbar());

    //Display run options
    mainVerticalLayout->addWidget(runManager);

    //Below create the tabs for psu/eth/labjack/...
    mainVerticalLayout->addWidget(create_window_tabs());

    setLayout(mainVerticalLayout);
}

QToolBar* MainLayout::create_toolbar()
{
    toolbar = new QToolBar;

    //Save config button
    QAction* save_cfg = toolbar->addAction("Save config");
    connect(save_cfg, SIGNAL(triggered()), configManager, SLOT(save_config()));

    //Load config button
    QAction* load_cfg = toolbar->addAction("Load config");
    connect(load_cfg, SIGNAL(triggered()), configManager, SLOT(load_config()));

    toolbar->addSeparator();

    /* Start/Stop button */
    startTestButton = toolbar->addAction("Start Test");
    startTestButton->setIcon(QIcon(":/icon/startButton.png"));
    startTestButton->setDisabled(true);
    connect(startTestButton, SIGNAL(triggered()), runManager, SLOT(start_run()));
    connect(runManager, SIGNAL(run_mode_changed(enum RunMode)), this, SLOT(set_start_button(enum RunMode)));
    connect(runManager, SIGNAL(isInvalid_changed(bool)), startTestButton, SLOT(setDisabled(bool)));

    stopTestButton = toolbar->addAction("Stop Test");
    stopTestButton->setIcon(QIcon(":/icon/stopButton.png"));
    stopTestButton->setDisabled(true);
    connect(stopTestButton, SIGNAL(triggered()), runManager, SLOT(stop_run()));
    connect(runManager, SIGNAL(run_mode_changed(enum RunMode)), this, SLOT(set_stop_button(enum RunMode)));
    connect(runManager, SIGNAL(isInvalid_changed(bool)), stopTestButton, SLOT(setDisabled(bool)));

    toolbar->addSeparator();

    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setIconSize(QSize(16, 16));

    detachTestButton = toolbar->addAction("Detach");
    connect(detachTestButton, SIGNAL(triggered()), this, SLOT(detach_tab()));

    return toolbar;
}

QTabWidget* MainLayout::create_window_tabs()
{
    windowTabs= new QTabWidget;

    //Power supplies
    windowTabs->addTab(new PSUTab(configManager, runManager), "PSU");

    //Labjack
    windowTabs->addTab(new LBJTab(configManager, runManager), "Labjack");

    //RF
    windowTabs->addTab(new RFTab(configManager, runManager), "RF");

    //PROG
    windowTabs->addTab(new PROGTab(configManager, runManager), "PROG");

    //ETH
    windowTabs->addTab(new ETHTab(configManager, runManager), "ETH");

    //SEQ
    windowTabs->addTab(new SEQTab(configManager, runManager), "SEQ");

    //FOX
    windowTabs->addTab(new FFOXTab(configManager, runManager), "FOX");

    //OSC
//    windowTabs->addTab(new OSCTab(configManager, runManager), "OSC");

    return windowTabs;
}

void MainLayout::detach_tab(void)
{
    DetachedWindow *win = new DetachedWindow(windowTabs,
                                windowTabs->currentWidget(),
                                windowTabs->tabText(windowTabs->currentIndex()));

    win->show();
}
void MainLayout::set_start_button(enum RunMode mode)
{
    switch(mode) {
    case Creation:
        startTestButton->setIcon(QIcon(":icon/startButton.png"));
        startTestButton->setDisabled(false);
        break;
    case StartRun:
        startTestButton->setIcon(QIcon(":icon/startButton_active.png"));
        startTestButton->setDisabled(true);
        break;
    case StopRun:
        startTestButton->setIcon(QIcon(":icon/startButton.png"));
        startTestButton->setDisabled(false);
        break;
    default:
        break;
    }
}

void MainLayout::set_stop_button(enum RunMode mode)
{
    switch(mode) {
    case Creation:
        stopTestButton->setIcon(QIcon(":icon/stopButton.png"));
        stopTestButton->setDisabled(true);
        break;
    case StartRun:
        stopTestButton->setIcon(QIcon(":icon/stopButton.png"));
        stopTestButton->setDisabled(false);
        break;
    case StopRun:
        stopTestButton->setIcon(QIcon(":icon/stopButton_active.png"));
        stopTestButton->setDisabled(true);
        break;
    default:
        break;
    }
}
