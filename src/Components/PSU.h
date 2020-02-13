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
    PSU(RunManager * runManager, const QString &element_name, const QString &address, const QString &vendor, uint channel_max, double voltage_max, double current_max);
    virtual ~PSU() override;

    enum vendor get_vendor() {return vd;}
    QVector<PSUChannel *> get_channel_list() const {return channel_list;}
    bool get_master() const {return master_set;}
    bool has_master() const {return master;}

    void set_config() override;

public slots:
    void set_master(int master_set);

    void update();
    void update_settings();
    void start();
    void stop();

signals:
    void master_changed(bool master_set);

protected:
    QString address;
    enum vendor vd;
    bool master;
    bool master_set;
    EthernetClient *eth;

    QTimer *log_timer;
    RunManager *runManager;
    bool is_logging;

    QVector<PSUChannel *> channel_list;


private:
    void init_ethernet(const QString &address);

    enum vendor check_vendor(const QString &vendor);
    QString check_vendor(enum vendor vd);

    void set_master_rohdeschwarz();

    void start_logging();
    void stop_logging();

    void init();

    QVector<QString> generate_header();
};

#endif // PSU_H
