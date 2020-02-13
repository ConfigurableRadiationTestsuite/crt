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

    connect(configManager, SIGNAL(loading_config()), this, SLOT(load_from_config()));
    connect(configManager, SIGNAL(saving_config()), this, SLOT(save_to_config()));

    create_layout();
}

WindowTab::~WindowTab() {
    clear_subwindow_list();
}

void WindowTab::create_layout() {
    mainVerticalLayout = new QVBoxLayout;

    SubWindow *window;
    foreach (window, subWindow_list) {
        /* Main Box for the window */
        QGroupBox * windowGroupBox = new QGroupBox(window->get_config_element()->get_element_name());
        connect(this, SIGNAL(clean_layout()), windowGroupBox, SLOT(deleteLater()));

        QHBoxLayout * window_horizontal_box = new QHBoxLayout;
        connect(this, SIGNAL(clean_layout()), window_horizontal_box, SLOT(deleteLater()));

        /* Create delete button */
        QPushButton * deleteButton = new QPushButton;
        deleteButton->setIcon(QIcon("../configurable-radiation-testsuite/icon/deleteButton.png"));
        deleteButton->setFixedSize(QSize(32,32));
        //Destroy the window
        connect(deleteButton, SIGNAL(clicked()), window, SLOT(deleteLater()));
        connect(this, SIGNAL(clean_layout()), deleteButton, SLOT(deleteLater()));
        //Clean up
        connect(window, SIGNAL(destroyed()), deleteButton, SLOT(deleteLater()));
        connect(window, SIGNAL(destroyed(SubWindow *)), this, SLOT(erase_window(SubWindow *)));

        /* Put everything together */
        window_horizontal_box->addWidget(window);
        window_horizontal_box->addWidget(deleteButton);

        windowGroupBox->setLayout(window_horizontal_box);
        mainVerticalLayout->addWidget(windowGroupBox);
    }

    /* Create add button */
    QPushButton * addButton = new QPushButton;
    addButton->setIcon(QIcon("../configurable-radiation-testsuite/icon/addButton.png"));
    addButton->setFixedSize(QSize(32,32));
    connect(addButton, SIGNAL(clicked()), this, SLOT(add_subwindow_from_dialog()));
    connect(this, SIGNAL(clean_layout()), addButton, SLOT(deleteLater()));

    mainVerticalLayout->addWidget(addButton);
    mainVerticalLayout->setAlignment(Qt::AlignTop);

    setLayout(mainVerticalLayout);
}

void WindowTab::update_layout() {
    emit clean_layout();
    delete mainVerticalLayout;
    delete layout();

    create_layout();
}

void WindowTab::erase_window(SubWindow *window) {
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

void WindowTab::clear_layout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clear_layout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            layout->removeWidget(item->widget());
           //delete item->widget();
        }
        delete item;
    }
}
