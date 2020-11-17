#ifndef FIELDFOX_H
#define FIELDFOX_H

/*
 * Author: Mattis Jaksch
 *
 */

class EthernetClient;

#include "Component.h"

class Fieldfox : public Component {
Q_OBJECT

public:
    Fieldfox(RunManager * runManager, const QString &config);
    Fieldfox(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &mode, ulong start_freq, ulong stop_freq, ulong points);
    virtual ~Fieldfox() override;

    QString get_address() const {return address;}
    ulong get_start_freq() const {return start_freq;}
    ulong get_stop_freq() const {return stop_freq;}
    ulong get_points() const {return points;}
    QVector<double> get_data() const {return data;}

    void set_config() override;

public slots:
    void init() override;
    void update() override;

    void set_start_freq(QString const &text);
    void set_stop_freq(QString const &text);
    void set_points(QString const &text);

signals:
    void data_available(const QVector<double> &);

private slots:
    void update_settings(bool ok);

private:
    QString address, mode;
    ulong start_freq, stop_freq, points;
    bool settings_ok = false;

    QVector<double> data;

    EthernetClient *eth;

    bool update_measure();

    void set_format();
    void set_trace(uint trace);

    void create_dummy_data(QVector<double> &data);

    QStringList generate_header() override;
};

#endif // FIELDFOX_H
