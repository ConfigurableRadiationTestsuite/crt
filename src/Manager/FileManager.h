#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/*
 * Author: Mattis Jaksch
 *
 * File manager to log values from the devices
 * hosted by the subwindows in the various tabs.
 * Components can be (de-)registered for logging.
 *
 */

class QFile;

#include <QVector>

struct ComponentFile {
    QString name;

    QFile * file;

    const void * subComponent;

    long long creationTime;
};

class FileManager {

public:
    FileManager();
    virtual ~FileManager();

    QVector<ComponentFile*> get_file_list() const {return file_list;}
    struct ComponentFile * get_file(const void * subComponent);

    void append_value_to_file(const void * subComponent, double value);
    void append_value_to_file(const void * subComponent, const QString &value);
    void append_values_to_file(const void * subComponent, const QVector<double> &values);
    void append_values_to_file(const void * subComponent, const QVector<QString> &values);

    void set_file_header(const void * subComponent, const QVector<QString> &header);

    void register_component(const void * subComponent, const QString name);
    void deregister_component(const void * subComponent);

    void set_root_directory(const QString &directory);

protected:
    QString root_directory;

    QVector<ComponentFile*> file_list;

    bool file_exists(const QFile * ref_file);

    QString vector_to_string(const QVector<double> &vector);
    QString vector_to_string(const QVector<QString> &vector);

private:
    QString escape_text(const QString &text);

    void update_root_directory();
};

#endif // FILEMANAGER_H
