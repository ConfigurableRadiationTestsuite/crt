#ifndef INDICATOR_H
#define INDICATOR_H

#include <QCheckBox>

class Indicator : public QCheckBox {
Q_OBJECT

public:
    Indicator(QIcon onIcon, QIcon offIcon, QIcon eventIcon);
    virtual ~Indicator();

public slots:
    void set_individual_icon(int);
    void set_event_icon(bool);
    void setDisabled(bool);

private:
    QIcon *lastIcon;
    QIcon *onIcon;
    QIcon *offIcon;
    QIcon *eventIcon;

};

#endif // INDICATOR_H
