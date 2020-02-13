#ifndef LABJACKCHANNEL_H
#define LABJACKCHANNEL_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface from a Labjack Window to a real
 * labjack (Here T7)
 *
 */

class QElapsedTimer;

#include <QWidget>

class LabjackChannel : public QWidget {
Q_OBJECT

public:
    LabjackChannel(QString const &name, int* handle, int p_chan, int n_chan);
    LabjackChannel(QString const &name, int* handle, int p_chan, int n_chan, int gain, double boundary);
    virtual ~LabjackChannel() override;

    void update_value(double value);

    double get_value() const {return external_gain*value;}
    QString get_name() const {return name;}
    int get_pchan() const {return p_chan;}
    int get_nchan() const {return n_chan;}
    double get_boundary() const {return boundary;}
    double get_gain() const {return external_gain;}

    int get_pchan_address();
    int get_nchan_address();
    int get_pchan_resolution_address();
    int get_nchan_resolution_address();
    int get_pchan_negative_address();
    int get_nchan_negative_address();
    int get_pchan_range_address();
    int get_nchan_range_address();
    int get_pchan_settling_address();
    int get_nchan_settling_address();

    void set_range();

public slots:
    void update();

    void set_boundary(const QString &text);
    void set_external_gain(const QString &text);
    void set_resolution(uint index);
    void set_settling(uint index);

signals:
    void value_changed(const QString &text);
    void boundary_check_failed();

private:
    QString name;
    int* handle;
    int p_chan, n_chan;
    double boundary, range;
    int external_gain;
    bool is_input, is_differential;
    double value;

    QElapsedTimer *changeTimer;

    QVector<double> range_list = {0.01, 0.1, 1, 10};

    void set_differential();

    bool check_boundary();

    int write(int address, const int TYPE, double value);
    int read(int address, const int TYPE, double &value);
};


inline int LabjackChannel::get_pchan_address() {return p_chan*2;}
inline int LabjackChannel::get_nchan_address() {return n_chan*2;}
inline int LabjackChannel::get_pchan_resolution_address() {return 41500 + p_chan;}
inline int LabjackChannel::get_nchan_resolution_address() {return 41500 + n_chan;}
inline int LabjackChannel::get_pchan_negative_address() {return 41000 + p_chan;}
inline int LabjackChannel::get_nchan_negative_address() {return 41000 + n_chan;}
inline int LabjackChannel::get_pchan_range_address() {return 40000 + 2*p_chan;}
inline int LabjackChannel::get_nchan_range_address() {return 40000 + 2*n_chan;}
inline int LabjackChannel::get_pchan_settling_address() {return 42000 + 2*p_chan;}
inline int LabjackChannel::get_nchan_settling_address() {return 42000 + 2*n_chan;}

#endif // LABJACKCHANNEL_H
