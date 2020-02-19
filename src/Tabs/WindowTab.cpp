#include "WindowTab.h"

#include "src/Dialog/SpecAddDialog.h"
#include "src/Configuration/ConfigManager.h"
#include "src/Configuration/ConfigElement.h"
#include "src/Manager/EventManager.h"
#include "src/SubWindow/SubWindow.h"

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

WindowTab::WindowTab(ConfigManager *configManager, EventManager *eventManager, RunManager *runManager)
    : configManager(configManager), eventManager(eventManager), runManager(runManager) {
    qDebug("Create windowtab" + (sectionName).toLatin1());

    connect(configManager, SIGNAL(loading_config()), this, SLOT(load_from_config()));
    connect(configManager, SIGNAL(saving_config()), this, SLOT(save_to_config()));

    create_layout();
}

WindowTab::~WindowTab() {
    qDebug("Destroy WindowTab: " + (sectionName).toLatin1());

    clear_subwindow_list();
    delete mainTabLayout;
}

void WindowTab::create_layout() {
    qDebug("Create tab layout");
    mainTabLayout = new QVBoxLayout;

    SubWindow *window;
    foreach (window, subWindow_list) {
        qDebug("Add window to tab");
        /* Main Box for the window */
        QGroupBox * windowGroupBox = new QGroupBox(window->get_config_element()->get_element_name());
        connect(this, SIGNAL(clean_layout()), windowGroupBox, SLOT(deleteLater()));

        QHBoxLayout * windowHLayout = new QHBoxLayout;
        connect(this, SIGNAL(clean_layout()), windowHLayout, SLOT(deleteLater()));

        /* Create delete button */
        QPushButton * deleteButton = new QPushButton;
        deleteButton->setIcon(QIcon("../configurable-radiation-testsuite/icon/deleteButton.png"));
        deleteButton->setFixedSize(QSize(32,32));
        //Destroy the window
        connect(deleteButton, SIGNAL(clicked()), window, SLOT(deleteLater()));
        connect(this, SIGNAL(clean_layout()), deleteButton, SLOT(deleteLater()));
        connect(window, SIGNAL(destroyed(SubWindow *)), this, SLOT(erase_window(SubWindow *)));

        /* Put everything together */
        windowHLayout->addWidget(window);
        windowHLayout->addWidget(deleteButton);

        windowGroupBox->setLayout(windowHLayout);
        mainTabLayout->addWidget(windowGroupBox);
    }

    /* Create add button */
    QPushButton * addButton = new QPushButton;
    addButton->setIcon(QIcon("../configurable-radiation-testsuite/icon/addButton.png"));
    addButton->setFixedSize(QSize(32,32));
    connect(addButton, SIGNAL(clicked()), this, SLOT(add_subwindow_from_dialog()));
    connect(this, SIGNAL(clean_layout()), addButton, SLOT(deleteLater()));

    mainTabLayout->addWidget(addButton);
    mainTabLayout->setAlignment(Qt::AlignTop);

    setLayout(mainTabLayout);
}

void WindowTab::update_layout() {
    emit clean_layout();
    delete mainTabLayout;
    delete layout();

    create_layout();
}

void WindowTab::erase_window(SubWindow *window) {
    qDebug("Erase singel window");
    for(QList<SubWindow *>::iterator it = subWindow_list.begin(); it != subWindow_list.end(); it++) {
        if((*it) == window) {
            subWindow_list.erase(it);
            break;
        }
    }

    update_layout();
}

void WindowTab::save_to_config() {
    SubWindow *window;
    foreach (window, subWindow_list)
            configManager->append_content(sectionName, window->get_config());
}

void WindowTab::clear_subwindow_list() {
    qDebug("Clear window list");
    SubWindow *window;
    foreach (window, subWindow_list)
        delete window;

    subWindow_list.clear();
}

void WindowTab::add_subwindow_from_dialog() {
    if(addDialog == nullptr)
        create_add_subwindow_dialog();

    addDialog->show();
}
