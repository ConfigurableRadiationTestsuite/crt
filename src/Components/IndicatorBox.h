#ifndef INDICATORBOX_H
#define INDICATORBOX_H

#include <QCheckBox>

class IndicatorBox : public QCheckBox {
Q_OBJECT

public:
    IndicatorBox(QIcon onIcon, QIcon offIcon, QIcon eventIcon);
    virtual ~IndicatorBox();

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

#endif
