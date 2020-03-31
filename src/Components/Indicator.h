#ifndef INDICATOR_H
#define INDICATOR_H

#include <QCheckBox>

class Indicator : public QCheckBox {
Q_OBJECT

public:
    Indicator(QIcon on, QIcon off, QIcon event);
    virtual ~Indicator();

public slots:
    void set_individual_icon(int);
    void set_event_icon();

private:
    QIcon *on;
    QIcon *off;
    QIcon *event;

};

#endif // INDICATOR_H
