#ifndef PSUCHANNEL_H
#define PSUCHANNEL_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface from a PSU Window to a real
 * powersupply
 *
 * Address example: 192.168.3.103::5025
 */

class EthernetClient;

#include <QWidget>

enum vendor {none, rohdeSchwarz, tti};

class PSUChannel : public QWidget {
Q_OBJECT

public:
    PSUChannel(uint number, EthernetClient * eth, enum vendor vd, double voltage_set, double current_set, double voltage_max, double current_max);
    virtual ~PSUChannel();

    uint get_number() const {return number;}
    bool get_enable() const {return enable;}
    bool get_trigger() const {return trigger;}

    double get_voltage_meas() {return voltage_meas;}
    double get_current_meas() {return current_meas;}
    double get_voltage_set() {return voltage_set;}
    double get_current_set() {return current_set;}
    double get_voltage_max() {return voltage_max;}
    double get_current_max() {return current_max;}

public slots:
    void set_enable(int enable);
    void set_trigger(int trigger);
    void set_voltage(const QString &voltage);
    void set_current(const QString &current);

    void update();
    void meas_voltage();
    void meas_current();


signals:
    void voltage_changed(const QString &text);
    void current_changed(const QString &text);
    void enable_changed(bool enable);

private:
    uint number;
    EthernetClient *eth;
    enum vendor vd;
    bool enable;
    bool trigger;

    double voltage_meas;
    double current_meas;
    double voltage_set;
    double current_set;
    double voltage_max;
    double current_max;

    void overcurrent_protection();

    void update_rohdeschwarz();
    void meas_voltage_rohdeschwarz();
    void meas_current_rohdeschwarz();

    void update_tti();
    void meas_voltage_tti();
    void meas_current_tti();
};

inline void PSUChannel::set_trigger(int trigger) {this->trigger = trigger;}

#endif // PSUCHANNEL_H
