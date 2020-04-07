#include "RFIO.h"

#include "src/Manager/RunManager.h"
#include "RFIOChannel.h"
#include "RFIOUpdater.h"

#include <QProcess>
#include <QThread>

RFIO::RFIO(RunManager * runManager, const QString &config) : runManager(runManager) {
    load_config(config);

    //Element name

    //Address

    //Channel

    init();
}

RFIO::RFIO(RunManager * runManager, const QString &m_element_name, const QString &address, int channel)
    : runManager(runManager), address(address), channel(channel) {

    this->element_name = m_element_name;

    init();
}

RFIO::~RFIO() {
    delete rfioUpdater;
    delete updateThread;
    delete process;
}

void RFIO::set_config() {}

void RFIO::init() {
    //Create channel
    for(int i = 0; i < channel; i++)
        channel_list.push_back(new RFIOChannel(runManager, element_name, i));

    //Create reception process
    process = new QProcess(this);

    /* Create and start update thread */
    updateThread = new QThread;
    rfioUpdater = new RFIOUpdater(port, process, &channel_list);

    rfioUpdater->moveToThread(updateThread);
    connect(rfioUpdater, SIGNAL(disconnected()), this, SLOT(reconnect()));
    connect(updateThread, SIGNAL(started()), rfioUpdater, SLOT(start_process()));
    connect(rfioUpdater, SIGNAL(finished()), updateThread, SLOT(quit()));
    connect(rfioUpdater, SIGNAL(finished()), rfioUpdater, SLOT(deleteLater()));
    connect(updateThread, SIGNAL(finished()), updateThread, SLOT(deleteLater()));
    updateThread->start();
}

void RFIO::reconnect() {
    //Delete old connection, clean process and set it up again (init()?)
}
