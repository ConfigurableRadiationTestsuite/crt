#include "LabjackChannel.h"

#include <LabJackM.h>

#include <QElapsedTimer>
#include <QtMath>

LabjackChannel::LabjackChannel(QString const &name, int* handle, int p_chan, int n_chan, int gain, double boundary) : name(name), handle(handle), p_chan(p_chan), n_chan(n_chan), boundary(boundary), external_gain(gain) {
    this->is_input = true;
    this->is_differential = n_chan == 199 ? false : true;

    if(is_differential)
        set_differential();

    changeTimer = new QElapsedTimer;
    changeTimer->start();
}

LabjackChannel::~LabjackChannel() {}

void LabjackChannel::update_value(double value) {
    this->value = value;
    check_boundary();
    emit value_changed(QString::number(get_value()));
}

void LabjackChannel::update() {
    read(get_pchan_address(), LJM_FLOAT32, value);
    set_range();

    check_boundary();
    emit value_changed(QString::number(get_value()));
}

void LabjackChannel::refresh_value() {
    emit value_refreshed(QString::number(get_value()));
}

void LabjackChannel::set_differential() {
    write(get_pchan_negative_address(), LJM_UINT16, n_chan);
}

bool LabjackChannel::check_boundary() {
    if(boundary == 0)
        return true;

    if(qFabs(boundary) < qFabs(get_value())) {
        emit boundary_check_failed();
        return false;
    }

    return true;
}

void LabjackChannel::set_boundary(const QString &text) {
    boundary = text.toDouble();
}

void LabjackChannel::set_external_gain(const QString &text) {
    external_gain = text.toInt();
}

void LabjackChannel::set_resolution(uint index) {
    assert(0 < index && index <= 10);
    write(get_pchan_resolution_address(), LJM_UINT16, index);
    write(get_nchan_resolution_address(), LJM_UINT16, index);
}


void LabjackChannel::set_settling(uint index) {
    assert(0 < index && index <= 50000);
    write(get_pchan_settling_address(), LJM_UINT16, index);
    write(get_nchan_settling_address(), LJM_UINT16, index);
}

void LabjackChannel::set_range() {
    double available_range;
    foreach (available_range, range_list) {
        if(value < 0.9 * available_range) {
            if(range == available_range)
                return ;
            else {
                //Write to address
                range = available_range;
                write(get_pchan_range_address(), LJM_FLOAT32, range);
                write(get_nchan_range_address(), LJM_FLOAT32, range);
                return ;
            }
        }
    }
}

int LabjackChannel::write(int address, const int TYPE, double value) {
    if(*handle <= 0)
        return -1;
    return LJM_eWriteAddress(*handle, address, TYPE, value);
}

int LabjackChannel::read(int address, const int TYPE, double &value) {
    if(*handle <= 0)
        return -1;
    return LJM_eReadAddress(*handle, address, TYPE, &value);
}


