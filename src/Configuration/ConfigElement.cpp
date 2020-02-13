#include "ConfigElement.h"

ConfigElement::ConfigElement() {}

ConfigElement::~ConfigElement() {}

QString ConfigElement::get_config() {
    QString config = "";

    for(QVector<struct config_entry>::iterator i = config_entry_list.begin(); i != config_entry_list.end(); i++)
        config += (*i).name + "=" + (*i).value + "\n";

    return config;
}

void ConfigElement::load_config(const QString &content) {
    QString line = "";

    for(QString::const_iterator i = content.begin(); i != content.end(); i++) {
          if((*i) == '\n') {
              QStringRef line_name(&line, 0, line.indexOf("="));
              QStringRef line_value(&line, line.indexOf("=")+1, line.size()-line.indexOf("=")-1);
              config_entry_list.push_back({line_name.toString(), line_value.toString()});
              line = "";
          }
          else
              line += (*i);
    }
}

bool ConfigElement::parse_config(const QVector<QString> &entries) {
    for(QVector<QString>::const_iterator i = entries.begin(); i != entries.end(); i++)
        if(get_entry((*i)) == nullptr)
            return false;

    return true;
}

struct config_entry * ConfigElement::get_entry(const QString &name) {
   for(QVector<struct config_entry>::iterator i = config_entry_list.begin(); i != config_entry_list.end(); i++)
        if((*i).name.contains(name))
            return &(*i);

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

void ConfigElement::set_config() {

}
