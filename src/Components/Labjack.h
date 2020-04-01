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
class RunManager;

class QElapsedTimer;

#include "src/Configuration/ConfigElement.h"

#include <QWidget>

class Labjack : public QWidget, public ConfigElement {
Q_OBJECT

public:
    Labjack(RunManager * runManager, const QString &config);
    Labjack(RunManager * runManager, const QString &m_element_name, const QString &channel_name, const QString &pchannel, const QString &nchannel);
    virtual ~Labjack() override;

    void set_config() override;

    int get_main_resolution_address();
    int get_main_range_address();
    int get_main_settling_address();

    QVector<LabjackChannel*> get_channel_list() const {return channel_list;}

public slots:
    void update();

    void set_main_settling(const QString &text);
    void set_main_resolution(int index);
    void set_samplerate(const QString &text);
    void set_maximum_samplerate(int is_maximum);

    void start_logging();
    void stop_logging();

signals:
    void samplerate_changed(const QString &text);

private:
    int handle;
    bool is_connected;
    RunManager * runManager;

    QVector<LabjackChannel*> channel_list;
    QVector<int> address_list;
    QVector<int> type_list;
    QVector<double> value_list;

    QTimer *timer;
    QElapsedTimer *sampleTimer;
    int samplerate = 1, maxSamplerate = 1;
    bool is_logging;
    bool is_maximum;

    void init();
    void open_labjack();

    int write(int address, const int TYPE, double value);
    int read(int address, const int TYPE, double &value);
    int read(const QVector<int> &address, const QVector<int> &TYPE, QVector<double> &value);

    void get_channel_addresses(const QString &input, QVector<int> &output);
    void get_channel_names(const QString &input, QVector<QString> &output);

    QVector<QString> generate_header();
};

inline int Labjack::get_main_resolution_address() {return 43903;}
inline int Labjack::get_main_range_address() {return 43900;}
inline int Labjack::get_main_settling_address() {return 43904;}

#endif // LABJACK_H
