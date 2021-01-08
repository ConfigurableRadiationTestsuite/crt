#ifndef PSUCHANNEL_H
#define PSUCHANNEL_H

/*
 * Author: Mattis Jaksch
 *
 * Single channel of a multi-channel OSC;
 *
 */

class EthernetClient;

#include <QWidget>

class OSCChannel : public QWidget {
Q_OBJECT

public:
    enum vendor {none, rigol};

    OSCChannel(uint number, EthernetClient *eth, enum vendor vd);
    ~OSCChannel() {}

    uint get_number() const {return number;}
    bool get_enable() const {return enable;}

    double get_voltage_meas() const {return voltage_meas;}
    double get_current_meas() const {return current_meas;}

public slots:
    void set_enable(int enable);
    void set_voltage(const QString &voltage);
    void set_current(const QString &current);

    bool update();
    void meas_voltage();
    void meas_current();

signals:
    void voltage_changed(const QString &text);
    void current_changed(const QString &text);
    void enable_changed(bool enable);
    void overcurrent_event();

private:
    uint number;
    EthernetClient *eth;
    enum vendor vd;
    bool enable = false;

    double voltage_meas = 0, current_meas = 0;

    /* VENDOR */
    //void update_vendor();
    //void meas_voltage_vendor();
    //void meas_current_vendor();
};

#endif // PSUCHANNEL_H
