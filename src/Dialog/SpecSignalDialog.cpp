#include "SpecSignalDialog.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

SpecSignalDialog::SpecSignalDialog() {
    okButton = new QPushButton("Ok", this);
    cancleButton = new QPushButton("Cancel", this);
}

SpecSignalDialog::~SpecSignalDialog() {
    delete okButton;
    delete cancleButton;
}

void SpecSignalDialog::create_dialog() {
    QVBoxLayout * buttonLayout = new QVBoxLayout(this);

    for(QVector<struct RegisteredSignalBox>::iterator i = registeredSignal_list.begin(); i != registeredSignal_list.end(); i++)
        buttonLayout->addWidget((*i).checkBox);

    QHBoxLayout * hlayout = new QHBoxLayout(this);
    hlayout->addWidget(okButton);
    hlayout->addWidget(cancleButton);

    connect(cancleButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout * vlayout = new QVBoxLayout(this);
    vlayout->addLayout(buttonLayout);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);
}

void SpecSignalDialog::add_entry(bool isChecked, struct RegisteredSignal *sig) {
    registeredSignal_list.push_back({new QCheckBox(sig->name, this), sig});
    registeredSignal_list[registeredSignal_list.size()-1].checkBox->setCheckState((isChecked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
}
