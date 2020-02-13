#include "SpecAddDialog.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

SpecAddDialog::SpecAddDialog() {
    okButton = new QPushButton("Ok", this);
    cancleButton = new QPushButton("Cancel", this);
}

SpecAddDialog::~SpecAddDialog() {}

void SpecAddDialog::create_dialog() {
    QFormLayout * formLayout = new QFormLayout;

    for(QVector<struct LineEntry>::iterator i = line_entry_list.begin(); i != line_entry_list.end(); i++)
        formLayout->addRow((*i).display_name, (*i).input_value);

    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(okButton);
    hlayout->addWidget(cancleButton);

    connect(cancleButton, SIGNAL(clicked()), this, SLOT(close()));

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
