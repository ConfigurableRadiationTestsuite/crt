#include "Ethernet.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTcpSocket>
#include <QTcpServer>

Ethernet::Ethernet(RunManager *runManager, const QString &config)
    : Component(runManager, config) {
    load_config(config);
    assert(parse_config({"name", "port", "timeout"}));

    elementName = get_value("name");
    port = get_value("port").toUInt();
    timeout = get_value("timeout").toLong();
}

Ethernet::Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout)
    : Component(m_element_name, runManager), port(port), timeout(timeout) {}

Ethernet::~Ethernet() {
    if(server != nullptr) {
        server->close();
        delete server;
    }
    if(socket != nullptr) {
        socket->close();
        delete socket;
    }

    delete timeoutTimer;
}

void Ethernet::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("port", QString::number(port));
    set_value("timeout", QString::number(timeout));
}

void Ethernet::init() {
    set_status(Waiting);

    timeoutTimer = new QTimer;
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(handle_disconnection()));

    connect(runManager, SIGNAL(run_name_changed(const QString &)), this, SLOT(set_data_folder()));

    if(runManager->is_valid())
        set_data_folder();

    emit init_done();
}

void Ethernet::start_logging() {
    /* Setup Server */
    server = new QTcpServer;
    connect(server, SIGNAL(newConnection()), this, SLOT(accept_connection()));

    if(!server->listen(QHostAddress::Any, port))
        set_status(Disconnected);
    else
        set_status(Connected);

    /* Setup Log */
    runManager->register_component(this, elementName);
    runManager->set_file_header(this, generate_header());

    runManager->set_run_mode(StartLog, elementName);
    logging = true;
    timeoutTimer->start(timeout);
    emit is_logging(true);
}

void Ethernet::stop_logging() {
    logging = false;
    timeoutTimer->stop();
    emit is_logging(false);
    set_status(Waiting);

    runManager->set_run_mode(StopLog, elementName);
    runManager->deregister_component(this);

    //Close socket and server
    socket->close();
    server->close();
}

void Ethernet::accept_connection() {
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(accept_data()));
}

void Ethernet::accept_data() {
    QByteArray data = socket->readAll();

    QString filename = data_folder + "/" + "Package_" + QString::number(QDateTime::currentMSecsSinceEpoch());

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();

    files += 1;
    emit files_changed(QString::number(files));
    bytes += data.size();
    emit bytes_changed(QString::number(bytes));

    runManager->append_values_to_file(this, {filename, QString::number(data.size())});

    reset_timeout();
}

void Ethernet::set_data_folder() {
    data_folder = runManager->get_root_directory() + "/" + elementName;

    if(!QDir(data_folder).exists())
        QDir().mkdir(data_folder);
}

void Ethernet::handle_disconnection() {
    emit connection_timed_out();
    set_status(Disconnected);
    timeoutTimer->stop();
}
