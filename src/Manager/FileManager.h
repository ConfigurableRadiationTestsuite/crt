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

#include <QFile>
#include <QVector>

struct ComponentFile {
    QString name;

    QFile * file;

    const void * subComponent;

    long long creationTime;
};

typedef struct ComponentFile Component;

class FileManager {

public:
    FileManager() {}
    virtual ~FileManager() {}

    QVector<Component*> get_file_list() const {return file_list;}
    Component * get_file(const void * subComponent);
    QString get_root_directory() const {return root_directory;}

    void append_value_to_file(const void * subComponent, double value);
    void append_value_to_file(const void * subComponent, const QString &value);
    void append_values_to_file(const void * subComponent, const QVector<double> &values);
    void append_values_to_file(const void * subComponent, const QStringList &values);

    void set_file_header(const void * subComponent, const QStringList &header);

    void register_component(const void * subComponent, const QString name);
    void deregister_component(const void * subComponent);

    void set_root_directory(const QString &directory);

protected:
    QString root_directory;

    QVector<Component*> file_list;

    bool file_exists(const QFile * ref_file);

    QString vector_to_string(const QVector<double> &vector);

private:
    QString escape_text(const QString &text);

    void update_root_directory();
};

inline bool FileManager::file_exists(const QFile * ref_file) {
    foreach (ComponentFile * file, file_list)
        if(file->file->fileName() == ref_file->fileName())
            return true;

    return false;
}

inline Component * FileManager::get_file(const void * subComponent) {
    foreach (Component * file, file_list)
        if(file->subComponent == subComponent)
            return file;

    return nullptr;
}

#endif // FILEMANAGER_H
