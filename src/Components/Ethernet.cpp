#include "Ethernet.h"

#include <QDateTime>
#include <QElapsedTimer>
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

    init();
}

Ethernet::Ethernet(RunManager *runManager, const QString &m_element_name, uint port, long timeout)
    : Component(m_element_name, runManager), port(port), timeout(timeout) {

    init();
}

Ethernet::~Ethernet() {}

void Ethernet::set_config() {
    config_entry_list.clear();

    set_value("name", elementName);
    set_value("port", QString::number(port));
    set_value("timeout", QString::number(timeout));
}

void Ethernet::init() {
    emit status_changed(Waiting);

    timer = new QElapsedTimer;

    connect(runManager, SIGNAL(run_name_changed(const QString &)), this, SLOT(set_data_folder()));

    if(runManager->is_valid())
        set_data_folder();
}

void Ethernet::update() {
    if(timer->isValid() && timer->elapsed() > timeout) {
        emit connection_timed_out();
        emit status_changed(Disconnected);
        timer->restart();
    }
}

void Ethernet::start_logging() {
    /* Setup Server */
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(accept_connection()));

    if(!server->listen(QHostAddress::Any, port))
        emit status_changed(Disconnected);
    else
        emit status_changed(Connected);

    /* Setup Log */
    runManager->register_component(this, elementName);
    runManager->set_file_header(this, generate_header());

    runManager->set_run_mode(StartLog, elementName);
    logging = true;
    timer->start();
    emit is_logging(true);
}

void Ethernet::stop_logging() {
    logging = false;
    timer->invalidate();
    emit is_logging(false);
    emit status_changed(Waiting);

    runManager->deregister_component(this);
    runManager->set_run_mode(StopLog, elementName);

    //Close socket and server
    socket->close();
    server->close();
    delete socket;
    delete server;
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

void Ethernet::reset_timeout() {
    timer->restart();
}

QStringList Ethernet::generate_header() {
    return {"File", "Bytes"};
}

void Ethernet::set_data_folder() {
    data_folder = runManager->get_root_directory() + "/" + elementName;

    if(!QDir(data_folder).exists())
        QDir().mkdir(data_folder);
}
