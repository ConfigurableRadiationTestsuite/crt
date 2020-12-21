#include "PSUChannel.h"

#include "EthernetClient.h"

#ifdef DUMMY_DATA
#include <QRandomGenerator>
#include <QtMath>
#endif

PSUChannel::PSUChannel(uint number, EthernetClient * eth, enum vendor vd, double voltage_set, double current_set, double voltage_max, double current_max) {
    this->number = number;
    this->eth = eth;
    this->vd = vd;

    this->voltage_max = voltage_max;
    this->current_max = current_max;
    this->voltage_set = voltage_set > voltage_max ? voltage_max : voltage_set;
    this->current_set = current_set > current_max ? current_max : current_set;
}

void PSUChannel::set_enable(int enable) {
    this->enable = enable == 0 ? false : true;

    if(!update())
        this->enable = false;

    emit enable_changed(this->enable);
}

void PSUChannel::set_voltage(const QString &voltage) {
    double dvoltage = voltage.toDouble();

    if(dvoltage > voltage_max)
        voltage_set = voltage_max;
    else
        voltage_set = dvoltage;

    update();

    emit voltage_changed(QString::number(voltage_set));
}

void PSUChannel::set_current(const QString & current) {
    double dcurrent = current.toDouble();

    if(dcurrent > current_max)
        current_set = current_max;
    else
        current_set = dcurrent;

    update();

    emit current_changed(QString::number(current_set));
}

void PSUChannel::overcurrent_protection() {
    if(current_set < current_meas*0.95) {
        if(trigger)
            set_enable(0);

        emit overcurrent_event();
    }
}

bool PSUChannel::update() {
    overcurrent_protection();

    if(vd == vendor::rohdeSchwarz)
        update_rohdeschwarz();

    if(vd == vendor::tti)
        update_tti();

//    if(vd == vendor::vendor)
//        update_vendor();
    return true;
}

void PSUChannel::meas_voltage() {
#ifdef DUMMY_DATA
    voltage_meas = voltage_set + double(QRandomGenerator::global()->bounded(-qint16(100), qint16(100))) / double(200);
    voltage_meas = voltage_meas < 0 ? 0 : voltage_meas;
    return ;
#endif

    if(vd == vendor::rohdeSchwarz)
        meas_voltage_rohdeschwarz();

    if(vd == vendor::tti)
        meas_voltage_tti();

//    if(vd == vendor::vendor)
//        meas_voltage_vendor();
}

void PSUChannel::meas_current() {
#ifdef DUMMY_DATA
    current_meas = (current_set/2) * (double(QRandomGenerator::global()->bounded(qint16(80), qint16(120))) / double(100));
    current_meas = current_meas < 0 || !enable ? 0 : current_meas;
    return ;
#endif

    if(vd == vendor::rohdeSchwarz)
        meas_current_rohdeschwarz();

    if(vd == vendor::tti)
        meas_current_tti();

//    if(vd == vendor::vendor)
//        meas_current_vendor();
}

void PSUChannel::update_rohdeschwarz() {
    if(eth->write("INST OUT" + QString::number(number + 1))) {
        eth->write(("VOLT " + QString::number(voltage_set)));
        eth->write(("CURR " + QString::number(current_set/1000.0)));
        eth->write("OUTP:CHAN " + QString(enable ? "ON" : "OFF"));
    }
}

void PSUChannel::meas_voltage_rohdeschwarz() {
/*    if(eth->write("INST OUT" + QString::number(number + 1))) {
        QString output;

        if(eth->query("MEAS:VOLT ?", output))
           voltage_meas = output.toDouble();
    }*/
    voltage_meas = 0;
}

void PSUChannel::meas_current_rohdeschwarz() {
/*    if(eth->write("INST OUT" + QString::number(number + 1))) {
        QString output;
        if(eth->query("MEAS:CURR ?", output))
            current_meas = output.toDouble()*1000;
    }*/
    current_meas = 0;
}

void PSUChannel::update_tti() {

}

void PSUChannel::meas_voltage_tti() {

}

void PSUChannel::meas_current_tti() {

}
