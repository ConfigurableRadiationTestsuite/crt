#ifndef SPECADDDIALOG_H
#define SPECADDDIALOG_H

/*
 * Author: Mattis Jaksch
 *
 * Dialog to add configurable elements to a tab.
 * Additional parameter forms can be passed down
 *
 */

class QLineEdit;

#include <QDialog>

struct LineEntry {
    QString display_name;
    QLineEdit* input_value;
};

class SpecAddDialog : public QDialog {
Q_OBJECT

public:
    SpecAddDialog();
    virtual ~SpecAddDialog();

    void add_entry(const QString &display_name);
    void add_entry(const QString &display_name, const QString &standard_value);
    void create_dialog();

    QPushButton* get_ok_button() {return okButton;}
    QVector<struct LineEntry> get_entry_list() const {return line_entry_list;}

private:
    QVector<struct LineEntry> line_entry_list;

    QPushButton* okButton;
    QPushButton* cancleButton;
};

#endif // PSUW_H
