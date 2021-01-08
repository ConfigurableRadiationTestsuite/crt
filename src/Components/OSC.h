#ifndef PSU_H
#define PSU_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface from a OSC Window to a real
 * oscilloscope
 *
 * Address example: 192.168.3.103::5025
 *
 */

class EthernetClient;

#include "Component.h"
#include "OSCChannel.h"

class OSC : public Component {
Q_OBJECT

public:
    OSC(RunManager * runManager, const QString &config);
    OSC(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &vendor, uint channel_max);
    virtual ~OSC() override;

    void set_config() override;

public slots:
    void init() override;
    void update() override;
    void update_settings();

    void switch_on();
    void switch_off();

signals:
    void disconnected(bool);

protected:
    QString address;
    uint channel_max = 0;
    EthernetClient *eth;

    QVector<OSCChannel *> channel_list;

private:
    bool check_network_connection();

    QStringList generate_header() override;
};

inline void OSC::update_settings() {
    foreach (OSCChannel * channel, channel_list)
        channel->update();
}

#endif // PSU_H
