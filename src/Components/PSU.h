#ifndef PSU_H
#define PSU_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface from a PSU Window to a real
 * powersupply
 *
 * Address example: 192.168.3.103::5025
 */

class EthernetClient;
class RunManager;

#include "PSUChannel.h"
#include "src/Configuration/ConfigElement.h"

#include <QWidget>

class PSU : public QWidget, public ConfigElement {
Q_OBJECT

public:
    PSU(RunManager * runManager, const QString &config);
    PSU(RunManager * runManager, const QString &m_element_name, const QString &address, const QString &vendor, uint channel_max, double voltage_max, double current_max);
    virtual ~PSU() override;

    enum vendor get_vendor() {return vd;}
    QVector<PSUChannel *> get_channel_list() const {return channel_list;}
    bool get_master_enable() const {return master_enable;}
    bool has_master_switch() const {return master_switch;}

    void set_config() override;

public slots:
    void set_master_enable(int master_enable);
    void set_master_trigger(int master_trigger);

    void update();
    void update_settings();

    void switch_on();
    void switch_off();
    void start_logging();
    void stop_logging();

signals:
    void master_changed(bool master_set);
    void disconnected(bool);

protected:
    RunManager *runManager;

    QString address;
    enum vendor vd;
    bool master_switch;
    bool master_enable = false;
    bool master_trigger = false;
    EthernetClient *eth;

    QTimer *log_timer;
    bool is_logging = false;

    QVector<PSUChannel *> channel_list;


private:
    void init_ethernet(const QString &address);

    enum vendor check_vendor(const QString &vendor);
    QString check_vendor(enum vendor vd);

    void set_master_rohdeschwarz();


    void init();

    QVector<QString> generate_header();
};

#endif // PSU_H
