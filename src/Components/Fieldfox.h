#ifndef FIELDFOX_H
#define FIELDFOX_H

/*
 * Author: Mattis Jaksch
 *
 * Keysight fieldfox is a network analyzer for
 * high frequency applications. Here the setup is
 * done and data is pulled
 *
 */

class EthernetClient;

#include "Component.h"

class Fieldfox : public Component
{
Q_OBJECT

public:
    Fieldfox(RunManager* runManager, const QString& config);
    Fieldfox(RunManager* runManager,
             const QString &m_element_name,
             const QString &address,
             const QString &mode,
             ulong start_freq,
             ulong stop_freq,
             uint datapoints);

    virtual ~Fieldfox() override;

    QString get_address() const
    {
        return address;
    }

    ulong get_start_freq() const
    {
        return start_freq;
    }

    ulong get_stop_freq() const
    {
        return stop_freq;
    }

    ulong get_datapoints() const
    {
        return datapoints;
    }

    QVector<double> get_data() const
    {
        return data;
    }

    void set_config() override;

public slots:
    void init() override;
    void update() override;

    void set_start_freq(const QString& text);
    void set_stop_freq(const QString& text);
    void set_points(const QString& text);

    void set_frequency_span(const QString& text);
    void set_frequency_resolution(const QString& text);
    void set_sweep_time(const QString& text);
    void set_mode(const QString& text);

signals:
    void data_available(const QVector<double>&);

private slots:
    void update_settings(bool connection_status);

private:
    QString address, mode;
    ulong start_freq, stop_freq;
    uint datapoints;
    bool settings_ok = false;

    QVector<double> data;

    EthernetClient* eth;

    bool update_measurement();

    void create_dummy_data(QVector<double>& data);

    QStringList generate_header() override;
};

#endif // FIELDFOX_H
