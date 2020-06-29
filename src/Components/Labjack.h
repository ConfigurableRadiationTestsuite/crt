#ifndef LABJACK_H
#define LABJACK_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface from a Labjack Window to a real
 * labjack (T7)
 *
 */

class LabjackChannel;

class QElapsedTimer;

#include "Component.h"

#include <LabJackM.h>

class Labjack : public Component {
Q_OBJECT

public:
    Labjack(RunManager * runManager, const QString &config);
    Labjack(RunManager * runManager, const QString &m_element_name, const QString &channel_name, int connectionType, const QString &identifier, const QString &pchannel, const QString &nchannel);
    virtual ~Labjack() override;

    void set_config() override;

    int get_main_resolution_address() const {return 43903;}
    int get_main_range_address() const {return 43900;}
    int get_main_settling_address() const {return 43904;}

    QVector<LabjackChannel*> get_channel_list() const {return channel_list;}

public slots:
    void update() override;

    void set_main_settling(const QString &text);
    void set_main_resolution(int index);
    void set_samplerate(const QString &text);
    void set_maximum_samplerate(int is_maximum);

signals:
    void samplerate_changed(const QString &text);

private:
    int handle;
    bool is_connected;
    int connectionType;
    QString identifier;

    QVector<LabjackChannel*> channel_list;
    QVector<int> address_list;
    QVector<int> type_list;
    QVector<double> value_list;

    QElapsedTimer *sampleTimer;
    QElapsedTimer *testTimer;
    int samplerate = 1, maxSamplerate = 1;
    bool is_maximum;

    void init();
    void open_labjack();

    int write(int address, const int TYPE, double value);
    int read(int address, const int TYPE, double &value);
    int read(const QVector<int> &address, const QVector<int> &TYPE, QVector<double> &value);

    void get_channel_addresses(const QString &input, QVector<int> &output);
    void get_channel_names(const QString &input, QVector<QString> &output);

    QStringList generate_header() override;

    void create_dummy_data(int size, double * values);
};

inline int Labjack::write(int address, const int TYPE, double value) {
    return is_connected ? LJM_eWriteAddress(handle, address, TYPE, value) : 0;
}

inline int Labjack::read(int address, const int TYPE, double &value) {
    return is_connected ? LJM_eReadAddress(handle, address, TYPE, &value) : 0;
}

inline void Labjack::set_maximum_samplerate(int is_maximum) {
    this->is_maximum = is_maximum > 0 ? true : false;
}

inline void Labjack::set_main_settling(const QString &text) {
    int index = text.toInt();
#ifdef DEBUG
    assert(0 < index && index <= 50000);
#endif
    write(get_main_settling_address(), LJM_FLOAT32, index);
}

inline void Labjack::set_main_resolution(int index) {
#ifdef DEBUG
    assert(0 < index && index <= 10);
#endif
    write(get_main_resolution_address(), LJM_UINT16, index);
}

#endif // LABJACK_H
