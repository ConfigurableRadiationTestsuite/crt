#ifndef PROGRAMMSTARTER_H
#define PROGRAMMSTARTER_H

/*
 * Author: Mattis Jaksch
 *
 */

class RunManager;

#include "src/Configuration/ConfigElement.h"

#include <QObject>

class ProgrammStarter : public QObject, public ConfigElement {
Q_OBJECT

public:
    ProgrammStarter(RunManager * runManager, const QString &config);
    ProgrammStarter(RunManager * runManager, const QString &name, const QString &path);
    virtual ~ProgrammStarter();

    void set_config() override;

public slots:
    void set_path(const QString &text);
    void set_early_logging(int early_logging);

    void start_logging();
    void stop_logging();

    void execute_programm();
    void kill_programm();


signals:
    void data_available(const QString &text);
    void started();
    void stopped();

private:
    QString path;
    bool is_early_logging;
};

#endif // PROGRAMMSTARTER_H
