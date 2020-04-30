#include "SpecAddDialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

SpecAddDialog::SpecAddDialog() {
    okButton = new QPushButton("Ok", this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(clicked_ok()));

    cancleButton = new QPushButton("Cancel", this);
    connect(cancleButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(cancleButton, SIGNAL(clicked()), this, SLOT(clicked_close()));
}

SpecAddDialog::~SpecAddDialog() {
    delete okButton;
    delete cancleButton;
}

void SpecAddDialog::create_dialog() {
    QFormLayout * formLayout = new QFormLayout;

    for(QVector<struct LineEntry>::iterator i = line_entry_list.begin(); i != line_entry_list.end(); i++)
        formLayout->addRow((*i).display_name, (*i).input_value);

    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(okButton);
    hlayout->addWidget(cancleButton);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addLayout(formLayout);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);
}

void SpecAddDialog::add_entry(const QString &display_name) {
    line_entry_list.push_back({display_name, new QLineEdit});
}

void SpecAddDialog::add_entry(const QString &display_name, const QString &standard_value) {
    QLineEdit * entry = new QLineEdit(standard_value);
    line_entry_list.push_back({display_name, entry});
}

void SpecAddDialog::clicked_ok() {
    emit dialog_accepted(true);
}

void SpecAddDialog::clicked_close()  {
    emit dialog_accepted(false);
}
