#include "PSUChannel.h"

#include "EthernetClient.h"

#include <QRandomGenerator>
#include <QtMath>

PSUChannel::PSUChannel(uint number, EthernetClient * eth, enum vendor vd, double voltage_set, double current_set, double voltage_max, double current_max) {
    this->number = number;
    this->eth = eth;
    this->vd = vd;

    this->voltage_max = voltage_max;
    this->current_max = current_max;
    this->voltage_set = voltage_set > voltage_max ? voltage_max : voltage_set;
    this->current_set = current_set > current_max ? current_max : current_set;
    this->voltage_meas = 0;
    this->current_meas = 0;

    enable = false;
    trigger = false;
}

PSUChannel::~PSUChannel() {}

void PSUChannel::set_enable(int enable) {
    this->enable = enable == 0 ? false : true;

    update();

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

void PSUChannel::update() {
    if(!eth->connectionOk()){
        eth->retry();
        return ;
    }

    overcurrent_protection();

    if(vd == vendor::rohdeSchwarz)
        update_rohdeschwarz();

    if(vd == vendor::tti)
        update_tti();

//    if(vd == vendor::vendor)
//        update_vendor();
}

void PSUChannel::meas_voltage() {
#ifdef DUMMY_DATA
    voltage_meas = voltage_set + QRandomGenerator::global()->bounded(-qint16(qPow(2, 10)), qint16(qPow(2, 10))) / qint16(qPow(2, 11));
    voltage_meas = voltage_meas < 0 ? 0 : voltage_meas;
    return ;
#endif

    if(!eth->connectionOk()){
        eth->retry();
        return ;
    }

    if(vd == vendor::rohdeSchwarz)
        meas_voltage_rohdeschwarz();

    if(vd == vendor::tti)
        meas_voltage_tti();

//    if(vd == vendor::vendor)
//        meas_voltage_vendor();
}

void PSUChannel::meas_current() {
#ifdef DUMMY_DATA
    current_meas = current_set/2 + QRandomGenerator::global()->bounded(-qint16(qPow(2, 10)), qint16(qPow(2, 10))) / qint16(qPow(2, 9));
    current_meas = current_meas < 0 ? 0 : current_meas;
    return ;
#endif

    if(!eth->connectionOk()) {
        eth->retry();
        return ;
    }

    if(vd == vendor::rohdeSchwarz)
        meas_current_rohdeschwarz();

    if(vd == vendor::tti)
        meas_current_tti();

//    if(vd == vendor::vendor)
//        meas_current_vendor();
}

void PSUChannel::update_rohdeschwarz() {
    if(eth->write("INST OUT" + std::to_string(number + 1))) {
        eth->write(("VOLT " + QString::number(voltage_set)).toStdString());
        eth->write(("CURR " + QString::number(current_set/1000.0)).toStdString());
        eth->write("OUTP:CHAN " + std::string(enable ? "ON" : "OFF"));
    }
}

void PSUChannel::meas_voltage_rohdeschwarz() {
    if(eth->write("INST OUT" + std::to_string(number + 1))) {
        QString output = QString::fromUtf8(eth->query("MEAS:VOLT ?").c_str());
        voltage_meas = output.toDouble();
    }
}

void PSUChannel::meas_current_rohdeschwarz() {
    if(eth->write("INST OUT" + std::to_string(number + 1))) {
        QString output = QString::fromUtf8(eth->query("MEAS:CURR ?").c_str());
        current_meas = output.toDouble();
    }
}

void PSUChannel::update_tti() {

}

void PSUChannel::meas_voltage_tti() {

}

void PSUChannel::meas_current_tti() {

}
