#include "RFIO.h"

#include "src/Manager/RunManager.h"
#include "RFIOChannel.h"
#include "RFIOUpdater.h"

#include <QProcess>
#include <QThread>

RFIO::RFIO(RunManager * runManager, const QString &config) : runManager(runManager) {
    load_config(config);

    assert(parse_config({"name", "address", "channel"}));
    this->element_name = get_value("name");
    this->address = get_value("address");
    this->channel = get_value("channel").toInt();

    for(int i = 0; i < channel; i++) {
        assert(parse_config({"c" + QString::number(i) + "margin"}));

        channel_list.push_back(new RFIOChannel(i, get_value("c" + QString::number(i) + "margin").toInt()));
        connect(channel_list[i], SIGNAL(error(QVector<int>, QVector<int>, int)), this, SLOT(handle_error(QVector<int>, QVector<int>, int)));
    }

    init();
}

RFIO::RFIO(RunManager * runManager, const QString &m_element_name, const QString &address, int channel)
    : runManager(runManager), address(address), channel(channel) {

    this->element_name = m_element_name;

    for(int i = 0; i < channel; i++) {
        channel_list.push_back(new RFIOChannel(i));
        connect(channel_list[i], SIGNAL(error(QVector<int>, QVector<int>, int)), this, SLOT(handle_error(QVector<int>, QVector<int>, int)));
    }

    init();
}

RFIO::~RFIO() {
    delete rfioUpdater;

    process->kill();
    updateThread->quit();
    delete process;
}

void RFIO::set_config() {
    config_entry_list.clear();

    set_value("name", element_name);
    set_value("address", address);
    set_value("channel", QString::number(channel));

    for(int i = 0; i < channel; i++)
        set_value("c" + QString::number(i) + "marge", QString::number(channel_list[i]->get_margin()));

}

void RFIO::init() {
    //Create reception process
    process = new QProcess(this);

    /* Create and start update thread */
    updateThread = new QThread;
    is_destroyed = false;
    rfioUpdater = new RFIOUpdater(port, process, &channel_list);

    rfioUpdater->moveToThread(updateThread);
    connect(updateThread, SIGNAL(started()), rfioUpdater, SLOT(start_process()));
    connect(rfioUpdater, SIGNAL(finished()), updateThread, SLOT(quit()));

    connect(rfioUpdater, SIGNAL(finished()), rfioUpdater, SLOT(deleteLater()));
    connect(updateThread, SIGNAL(finished()), updateThread, SLOT(deleteLater()));
    connect(updateThread, SIGNAL(finished()), process, SLOT(deleteLater()));

    connect(updateThread, SIGNAL(destroyed()), this, SLOT(set_thread_destroyed()));

    updateThread->start();
}

void RFIO::set_thread_destroyed() {
    is_destroyed = true;
}

void RFIO::reconnect() {
    //Delete old connection, clean process and set it up again (init()?)
    if(is_destroyed)
        init();
}

void RFIO::set_single_shot() {
    is_single_shot = true;
}

void RFIO::set_multi_shot() {
    is_single_shot = false;
}

void RFIO::start_logging() {
    qDebug("Start Log: " + element_name.toLatin1());

    runManager->register_component(this, element_name);
    runManager->set_file_header(this, generate_header());

    is_logging = true;
}

QVector<QString> RFIO::generate_header() {
    QVector<QString> header;
    header.push_back("Sample");

    RFIOChannel * channel;
    foreach (channel, channel_list) {
        header.push_back("I" + QString::number(channel->get_channel_number()));
        header.push_back("Q" + QString::number(channel->get_channel_number()));
    }

    return header;
}

void RFIO::stop_logging() {
    is_logging = false;

    runManager->deregister_component(this);
}

void RFIO::handle_error(QVector<int> i_data, QVector<int> q_data, int number) {
    /* Write the raw data to the runManager */
    if(!is_logging)
        return;

    if(is_single_shot) {
        process->kill();
        updateThread->quit();
    }

    for(int i = 0; i < i_data.size(); i++) {
        QVector<double> row;
        row.push_back(i);

        for(int j = 0; j < channel_list.size(); j++) {
            if(j == number) {
                row.push_back(i_data[i]);
                row.push_back(q_data[i]);
            }
            else {
                row.push_back(0);
                row.push_back(0);
            }
        }
        runManager->append_values_to_file(this, row);
    }
}

