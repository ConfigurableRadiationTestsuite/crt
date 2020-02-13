#ifndef SPECTRIGGERDIALOG_H
#define SPECTRIGGERDIALOG_H

/*
 * Author: Mattis Jaksch
 *
 * Dialog to add signals to an element
 * these signals will be triggered under certain conditions
 * leading to actions in the choosen elements
 *
 */

class QCheckBox;
class QPushButton;

#include "src/Manager/EventManager.h"

#include <QDialog>

struct RegisteredSignalBox {
    QCheckBox *checkBox;

    struct RegisteredSignal *sig;
};

class SpecTriggerDialog : public QDialog {
Q_OBJECT

public:
    SpecTriggerDialog();
    virtual ~SpecTriggerDialog();

    void add_entry(bool isChecked, struct RegisteredSignal *sig);
    void create_dialog();

    QPushButton* get_ok_button() {return okButton;}
    QVector<struct RegisteredSignalBox> get_registeredSignal_list() const {return registeredSignal_list;}

private:
    QVector<struct RegisteredSignalBox> registeredSignal_list;

    QPushButton* okButton;
    QPushButton* cancleButton;
};

#endif // SPECTRIGGERDIALOG_H
