#ifndef INDICATORICON_H
#define INDICATORICON_H

#include <QLabel>

class IndicatorIcon : public QLabel {
Q_OBJECT

public:
    IndicatorIcon(const QString &name, QPixmap connected, QPixmap disconnected, QPixmap event);
    virtual ~IndicatorIcon();

public slots:
    void set_event();
    void set_connected(int);

private slots:
    void reset_icon();

private:
    QString name;
    QPixmap connected;
    QPixmap disconnected;
    QPixmap event;
    QPixmap last;

    QTimer *eventTimer;

    void setNewPixmap(const QPixmap &map);
};

inline void IndicatorIcon::setNewPixmap(const QPixmap &map) {
    setPixmap(map);
    last = map;
}
#endif