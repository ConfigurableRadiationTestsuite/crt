#include "ConfigManager.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

const QString ConfigManager::sectionStart = "Section";
const QString ConfigManager::sectionEnd = "EndSection";

bool ConfigManager::get_config_section(QString name, QString &section) {
    QFile file(configName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    QString line;
    int pos = 0;

    while(!in.atEnd()) {
        line = in.readLine();

        if(line.contains(sectionStart) && !line.contains(sectionEnd))
            if(line.contains(name)) {
                //Check section position
                if(check_section_position(name, pos))
                    break;
                else
                    pos++;
            }
    }

    //Exit if the section was not found
    if(in.atEnd()) {
        file.close();
        return false;
    }

    while(!in.atEnd()) {
        line = in.readLine();

        if(line.contains(sectionEnd))
            break;
        else
            section += line + "\n";
    }

    file.close();

    return true;
}

/* Check how many sections of the same kind have already been found */
bool ConfigManager::check_section_position(const QString &name, int pos) {
    for(QVector<Section>::iterator i = section_positions.begin(); i != section_positions.end(); i++) {
        if((*i).sectionName == name) {
            if((*i).pos < pos) {
                (*i).pos++;
                return true;
            }
            else
                return false;
        }
    }

    //Add if the section is not yet in the list
    section_positions.push_back({name, pos});
    return true;
}

void ConfigManager::load_config() {
    /* Show dialog */
    QString tmp = QFileDialog::getOpenFileName(this, tr("Open Directory"), "./");

    if(QFileInfo::exists(tmp))
        configName = tmp;
    else
        return ;

    //Validate config
    assert(parse_config());

    section_positions.clear();

    emit loading_config();
}

void ConfigManager::save_config() {
    //Show dialog
    QFileDialog fileDialog(this, tr("Open File"), "./");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    QStringList tmp;

    if(fileDialog.exec())
        tmp = fileDialog.selectedFiles();
    else
        return ;

    if(tmp.size() > 0)
        configName = tmp[0];

    //Empty config content
    content.clear();
    content = "";

    //Emit saving config signal so all the (sub)components can pass their input
    emit saving_config();

    //Save config
    QFile file(configName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream in(&file);
    in << content;
    file.close();
}

bool ConfigManager::parse_config() {
    bool in_section = false;

    QFile file(configName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString line;

    while(!in.atEnd()) {
        line = in.readLine();

        if(line == "\n" || line.size() <= 1)
            continue;

        //Check if there is code outside of sections
        if(!in_section && !line.contains(sectionStart) && !line.contains(sectionEnd))
            return false;

        //Check if a 'Section' is followed by 'EndSection'
        if(!in_section && line.contains(sectionEnd))
            return false;
        if(in_section && !line.contains(sectionEnd) && line.contains(sectionStart))
            return false;

        //Get into section
        if(line.contains(sectionStart) && !line.contains(sectionEnd)) {
            in_section = true;

            //Check if section has a name of at least 3 characters
            if(line.length() < QString(sectionStart).length()+3)
                return false;
        }

        if(line.contains(sectionEnd))
            in_section = false;
    }

    return true;
}
