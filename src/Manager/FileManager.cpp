#include "FileManager.h"

#include <QDateTime>
#include <QDir>
#include <QFile>

FileManager::FileManager() {}

FileManager::~FileManager() {}

void FileManager::set_root_directory(const QString &directory) {
    QDir dir(directory);

    if(!dir.exists())
        dir.mkdir(directory);

    root_directory = directory;
}

void FileManager::register_component(const void * subComponent, const QString name) {
    qDebug("Register Log Component: " + name.toLatin1());

    QString filename = root_directory + "/" + name + "_" + QString::number(QDateTime::currentSecsSinceEpoch());
    QFile * new_file = new QFile(filename);

    assert(new_file->open(QIODevice::WriteOnly | QIODevice::Append));

    file_list.push_back(new ComponentFile{filename, new_file, subComponent, QDateTime::currentSecsSinceEpoch()});
}

void FileManager::deregister_component(const void * subComponent) {
    qDebug("DeRegister Log Component");

    for(int i = 0; i < file_list.size(); i++) {
        if(file_list[i]->subComponent == subComponent) {
            qDebug("Remove: " + (file_list[i]->name).toLatin1());

            file_list[i]->file->close();
            delete file_list[i]->file;
            delete file_list[i];
            file_list.remove(i);

            return ;
        }
    }
}

bool FileManager::file_exists(const QFile * ref_file) {
    ComponentFile * file;
    foreach (file, file_list)
        if(file->file->fileName() == ref_file->fileName())
            return true;

    return false;
}

void FileManager::append_value_to_file(const void * subComponent, double value) {
    long long time = QDateTime::currentMSecsSinceEpoch() - get_file(subComponent)->creationTime*1000;

    get_file(subComponent)->file->write((QString::number(time) + ";" + QString(QString::number(value) + "\n")).toUtf8());
    get_file(subComponent)->file->flush();
}

void FileManager::append_value_to_file(const void * subComponent, const QString &text) {
    long long time = QDateTime::currentMSecsSinceEpoch() - get_file(subComponent)->creationTime*1000;

    get_file(subComponent)->file->write((QString::number(time) + ";" + QString(text + "\n")).toUtf8());
    get_file(subComponent)->file->flush();
}

void FileManager::append_values_to_file(const void * subComponent, const QVector<double> &values) {
    long long  time = QDateTime::currentMSecsSinceEpoch() - get_file(subComponent)->creationTime*1000;

    QVector<double> internal_values;
    internal_values.push_back(time);
    internal_values.append(values);

    get_file(subComponent)->file->write((QString(vector_to_string(internal_values) + "\n")).toUtf8());
    get_file(subComponent)->file->flush();
}

void FileManager::set_file_header(const void * subComponent, const QVector<QString> &header) {
    QVector<QString> internal_header = {"Time"};
    internal_header.append(header);

    get_file(subComponent)->file->write((vector_to_string(internal_header) + "\n").toUtf8());
    get_file(subComponent)->file->flush();
}

struct ComponentFile * FileManager::get_file(const void * subComponent) {
    ComponentFile * file;
    foreach (file, file_list)
        if(file->subComponent == subComponent)
            return file;

    qDebug("Component is not registered!");

    return nullptr;
}

QString FileManager::vector_to_string(const QVector<double> &vector) {
    QString msg = "";
    QVectorIterator<double> it(vector);
    while(it.hasNext())
        msg += QString::number(it.next()) + (it.hasNext() ? ";" : "");

    return msg;
}

QString FileManager::vector_to_string(const QVector<QString> &vector) {
    QString msg = "";
    QVectorIterator<QString> it(vector);
    while(it.hasNext())
        msg += it.next() + (it.hasNext() ? ";" : "");

    return msg;
}

