#include "ConfigElement.h"

ConfigElement::ConfigElement() {
    qDebug("Create ConfigElement");
}

ConfigElement::~ConfigElement() {
    qDebug("Destroy ConfigElement");
}

QString ConfigElement::get_config() {
    QString config = "";

    for(QVector<struct config_entry>::iterator it = config_entry_list.begin(); it != config_entry_list.end(); it++)
        config += (*it).name + "=" + (*it).value + "\n";

    return config;
}

void ConfigElement::load_config(const QString &content) {
    QString line = "";

    for(QString::const_iterator it = content.begin(); it != content.end(); it++) {
          if((*it) == '\n') {
              QStringRef line_name(&line, 0, line.indexOf("="));
              QStringRef line_value(&line, line.indexOf("=")+1, line.size()-line.indexOf("=")-1);
              config_entry_list.push_back({line_name.toString(), line_value.toString()});
              line = "";
          }
          else
              line += (*it);
    }
}

bool ConfigElement::parse_config(const QVector<QString> &entries) {
    for(QVector<QString>::const_iterator it = entries.begin(); it != entries.end(); it++)
        if(get_entry((*it)) == nullptr)
            return false;

    return true;
}

struct config_entry * ConfigElement::get_entry(const QString &name) {
   for(QVector<struct config_entry>::iterator it = config_entry_list.begin(); it != config_entry_list.end(); it++)
        if((*it).name.contains(name))
            return &(*it);

    return nullptr;
}

void ConfigElement::set_value(const QString &name, const QString &value) {
    struct config_entry * entry = get_entry(name);

    if(entry == nullptr)
        config_entry_list.push_back({name, value});
    else
        entry->value = value;
}

QString ConfigElement::get_value(const QString &name) {
    if(get_entry(name) != nullptr)
        return get_entry(name)->value;
    else
        return "";
}
