#ifndef LABJACK_H
#define LABJACK_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface from a Labjack Window to a real
 * labjack
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
    Labjack(RunManager * runManager, const QString &name, const QString &type, const QString &channel_name, const QString &pchannel, const QString &nchannel);
    virtual ~Labjack() override;

    void set_config() override;

    int get_main_resolution_address();
    int get_main_range_address();
    int get_main_settling_address();

    QVector<LabjackChannel*> get_channel_list() const {return channel_list;}

public slots:
    void set_main_settling(const QString &text);
    void set_main_resolution(int index);
    void set_samplerate(const QString &text);
    void set_maximum_samplerate(int is_maximum);

    void update();

    void start();
    void stop();

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
    int samplerate, maxSamplerate;
    bool is_logging;
    bool is_maximum;

    void init();
    void open_labjack();

    int write(int address, const int TYPE, double value);
    int read(int address, const int TYPE, double &value);
    //int write(const QVector<int> &address, const QVector<const int> &TYPE, const QVector<double> &value);
    int read(const QVector<int> &address, const QVector<int> &TYPE, QVector<double> &value);

    void start_logging();
    void stop_logging();
    QVector<QString> generate_header();
};

inline int Labjack::get_main_resolution_address() {return 43903;}
inline int Labjack::get_main_range_address() {return 43900;}
inline int Labjack::get_main_settling_address() {return 43904;}

#endif // LABJACK_H
