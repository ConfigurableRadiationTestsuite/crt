#include "SpecTriggerDialog.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

SpecTriggerDialog::SpecTriggerDialog() {
    okButton = new QPushButton("Ok", this);
    cancleButton = new QPushButton("Cancel", this);
}

SpecTriggerDialog::~SpecTriggerDialog() {}

void SpecTriggerDialog::create_dialog() {
    QVBoxLayout * buttonLayout = new QVBoxLayout;

    for(QVector<struct RegisteredSignalBox>::iterator i = registeredSignal_list.begin(); i != registeredSignal_list.end(); i++)
        buttonLayout->addWidget((*i).checkBox);

    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(okButton);
    hlayout->addWidget(cancleButton);

    connect(cancleButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addLayout(buttonLayout);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);
}

void SpecTriggerDialog::add_entry(bool isChecked, struct RegisteredSignal *sig) {
    registeredSignal_list.push_back({new QCheckBox(sig->name, this), sig});
    registeredSignal_list[registeredSignal_list.size()-1].checkBox->setCheckState((isChecked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
}
