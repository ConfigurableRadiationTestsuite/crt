#include "PSUChannel.h"

#include "EthernetClient.h"

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

PSUChannel::~PSUChannel() {

}

void PSUChannel::set_enable(int enable) {
    this->enable = enable == 0 ? false : true;

    emit enable_changed(this->enable);

    update();
}

void PSUChannel::set_voltage(const QString &voltage) {
    double dvoltage = voltage.toDouble();

    if(dvoltage > voltage_max)
        voltage_set = voltage_max;
    else
        voltage_set = dvoltage;

    emit voltage_changed(QString::number(voltage_set));

    update();
}

void PSUChannel::set_current(const QString & current) {
    double dcurrent = current.toDouble();

    if(dcurrent > current_max)
        current_set = current_max;
    else
        current_set = dcurrent;

    emit current_changed(QString::number(current_set));

    update();
}

void PSUChannel::overcurrent_protection() {

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
}

void PSUChannel::meas_voltage() {
    if(!eth->connectionOk()){
        eth->retry();
        return ;
    }

    if(vd == vendor::rohdeSchwarz)
        meas_voltage_rohdeschwarz();

    if(vd == vendor::tti)
        meas_voltage_tti();
}

void PSUChannel::meas_current() {
    if(!eth->connectionOk()) {
        eth->retry();
        return ;
    }

    if(vd == vendor::rohdeSchwarz)
        meas_current_rohdeschwarz();

    if(vd == vendor::tti)
        meas_current_tti();
}

void PSUChannel::update_rohdeschwarz() {
    if(eth->write("INST OUT" + std::to_string(number + 1))) {
        eth->write("VOLT " + std::to_string(voltage_set));
        eth->write("CURR " + std::to_string(current_set/1000));
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
