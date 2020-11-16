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

    void set_config() override;

public slots:
    void init() override;
    void update() override;

signals:

private:
    QString address, mode;
    ulong start_freq, stop_freq, points;

    EthernetClient *eth;

    QVector<int> measure();

    QStringList generate_header() override;
};

#endif // FIELDFOX_H
