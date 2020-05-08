#include "SpecSignalDialog.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

SpecSignalDialog::SpecSignalDialog() {}

SpecSignalDialog::~SpecSignalDialog() {}

void SpecSignalDialog::create_dialog() {
    QVBoxLayout * buttonLayout = new QVBoxLayout(this);

    for(QVector<struct RegisteredSignalBox>::iterator i = registeredSignal_list.begin(); i != registeredSignal_list.end(); i++)
        buttonLayout->addWidget((*i).checkBox);

    QHBoxLayout * hlayout = new QHBoxLayout;

    QPushButton *okButton = new QPushButton("Ok");
    connect(okButton, SIGNAL(clicked()), this, SLOT(accpect()));
    hlayout->addWidget(okButton);

    QPushButton *cancleButton = new QPushButton("Cancel");
    connect(cancleButton, SIGNAL(clicked()), this, SLOT(reject()));
    hlayout->addWidget(cancleButton);

    connect(cancleButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addLayout(buttonLayout);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);
}

void SpecSignalDialog::add_entry(bool isChecked, struct RegisteredSignal *sig) {
    registeredSignal_list.push_back({new QCheckBox(sig->name, this), sig});
    registeredSignal_list[registeredSignal_list.size()-1].checkBox->setCheckState((isChecked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
}
