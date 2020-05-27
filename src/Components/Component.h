#ifndef COMPONENT_H
#define COMPONENT_H

/*
 * Author: Mattis Jaksch
 *
 */

#include "src/Configuration/ConfigElement.h"
#include "src/Manager/RunManager.h"

#include <QObject>
#include <QTimer>

class Component : public QObject, public ConfigElement {
Q_OBJECT

public:
    Component(const QString &elementName, RunManager *runManager, uint time=1000);
    Component(RunManager *runManager, const QString &config, uint time=1000);
    virtual ~Component() override;

    QString get_element_name() const {return elementName;}

public slots:
    virtual void update() = 0;

    void start_logging();
    void stop_logging();

    void set_early_logging(int);

signals:
    void is_logging(bool);

    void data_available();

protected:
    RunManager *runManager;
    QString elementName;

    QTimer *logTimer = nullptr;
    bool logging = false;
    bool early_logging = false;

    virtual QVector<QString> generate_header() = 0;

    void configure_timer(uint time);
};

#endif // COMPONENT_H
