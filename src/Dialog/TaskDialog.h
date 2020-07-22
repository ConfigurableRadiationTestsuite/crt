#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include "src/Manager/EventManager.h"

#include <QDialog>

class TaskDialog : public QDialog {
Q_OBJECT

public:
    TaskDialog(const QVector<RegisteredSignal*> &signal_list);

    RegisteredSignal * get_signal() const {return selectedSignal;}

private slots:
    void set_selected_signal(RegisteredSignal *sig) {selectedSignal = sig;}

private:
    QVector<RegisteredSignal*> signal_list;
    RegisteredSignal *selectedSignal;

    void create_layout();
};

#include <QRadioButton>

class TaskRadioButton : public QRadioButton {
Q_OBJECT

public:
    TaskRadioButton(RegisteredSignal * sig)
        : sig(sig) {
        this->setText(sig->name);
        connect(this, SIGNAL(clicked()), this, SLOT(announce_signal()));
    }

signals:
    void toggled(RegisteredSignal *);

private slots:
    void announce_signal() {emit toggled(sig);}

private:
    RegisteredSignal *sig;
};

#endif // TASKDIALOG_H
