#ifndef CONFIGELEMENT_H
#define CONFIGELEMENT_H

/*
 * Author: Mattis Jaksch
 *
 * Abstract class to provide an interface for all
 * elements that have a section in the config file
 * (provided by the ConfigManager)
 *
 */

#include <QString>
#include <QVector>

struct config_entry {
    QString name;
    QString value;
};

typedef struct config_entry Entry;

class ConfigElement {
public:
    ConfigElement() {}
    virtual ~ConfigElement() {}

    QString get_config();
    QString get_value(const QString &name);

    void set_value(const QString &name, const QString &value);
    virtual void set_config() = 0;
    void load_config(const QString &content);

    bool is_empty() const {return config_entry_list.empty();}

protected:
    QVector<Entry> config_entry_list;

    Entry *get_entry(const QString &name);

    virtual bool parse_config(const QVector<QString> &entries);
};

inline QString ConfigElement::get_value(const QString &name) {
    return (get_entry(name) != nullptr) ? get_entry(name)->value : "";
}

inline void ConfigElement::set_value(const QString &name, const QString &value) {
    Entry *entry = get_entry(name);

    if(entry == nullptr)
        config_entry_list.push_back({name, value});
    else
        entry->value = value;
}

inline Entry *ConfigElement::get_entry(const QString &name) {
   for(QVector<struct config_entry>::iterator it = config_entry_list.begin(); it != config_entry_list.end(); it++)
        if((*it).name.contains(name))
            return &(*it);

    return nullptr;
}

#endif // CONFIGELEMENT_H
