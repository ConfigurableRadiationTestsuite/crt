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
    Component(const QString &elementName, RunManager *runManager);
    Component(RunManager *runManager, const QString &config);
    virtual ~Component() override;

    QString get_element_name() const {return elementName;}

public slots:
    virtual void update() = 0;

    void start_logging();
    void stop_logging();

protected:
    RunManager *runManager;
    QString elementName;

    QTimer *logTimer;
    bool logging = false;
    bool early_logging = false;

    virtual QVector<QString> generate_header() = 0;
};

#endif // COMPONENT_H
