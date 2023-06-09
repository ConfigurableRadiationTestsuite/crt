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

struct config_entry
{
    QString name;
    QString value;
};

class ConfigElement
{
public:
    ConfigElement() {}
    virtual ~ConfigElement() {}

    QString get_config();
    QString get_value(const QString& name, const QString& defaultvalue = "");

    void set_value(const QString& name, const QString& value);
    virtual void set_config() = 0;

    bool is_empty() const {return config_entry_list.empty();}

    void load_config(const QString& content);

protected:
    QVector<config_entry> config_entry_list;

    config_entry* get_entry(const QString& name);

    virtual bool parse_config(const QVector<QString>& entries);
};

#endif // CONFIGELEMENT_H

