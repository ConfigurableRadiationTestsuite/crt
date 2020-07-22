#include "TaskDialog.h"

#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>

TaskDialog::TaskDialog(const QVector<RegisteredSignal*> &signal_list)
    : signal_list(signal_list) {
    create_layout();
}

void TaskDialog::create_layout() {
    QVBoxLayout *signalLayout = new QVBoxLayout;

    /* Buttons and signals */
    QButtonGroup *buttonGroup = new QButtonGroup;

    foreach (RegisteredSignal *signal, signal_list) {
        if(signal->st == SignalType::start_log || signal->st == SignalType::stop_log)
            continue;

        TaskRadioButton *button = new TaskRadioButton(signal);
        connect(button, SIGNAL(toggled(RegisteredSignal *)), this, SLOT(set_selected_signal(RegisteredSignal *)));

        buttonGroup->addButton(button);
        signalLayout->addWidget(button);
    }

    TaskRadioButton *button = new TaskRadioButton(new RegisteredSignal{"sleep", SignalType::special, nullptr, nullptr});
    connect(button, SIGNAL(toggled(RegisteredSignal *)), this, SLOT(set_selected_signal(RegisteredSignal *)));

    /* Ok - Cancel */
    QHBoxLayout * hlayout = new QHBoxLayout;

    QPushButton *okButton = new QPushButton("Ok");
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    hlayout->addWidget(okButton);

    QPushButton *cancleButton = new QPushButton("Cancel");
    connect(cancleButton, SIGNAL(clicked()), this, SLOT(reject()));
    hlayout->addWidget(cancleButton);

    buttonGroup->addButton(button);
    signalLayout->addWidget(button);
    signalLayout->addLayout(hlayout);

    setLayout(signalLayout);
}
