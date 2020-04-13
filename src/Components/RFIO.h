#ifndef RFIO_H
#define RFIO_H

/*
* Author: Mattis Jaksch
*
*/

class RFIOChannel;
class RFIOUpdater;
class RunManager;

class QProcess;
class QThread;

#include "src/Configuration/ConfigElement.h"

#include <QObject>

class RFIO : public QObject, public ConfigElement {
Q_OBJECT

public:
   RFIO(RunManager * runManager, const QString &config);
   RFIO(RunManager * runManager, const QString &m_element_name, const QString &address, int channel);
   virtual ~RFIO() override;

   QVector<RFIOChannel *> get_channel_list() const {return channel_list;}

   void set_config() override;

public slots:
   void start_logging();
   void stop_logging();

private slots:
   void reconnect();

private:
   RunManager *runManager;
   QString address;
   int port;

   RFIOUpdater *rfioUpdater;
   QProcess *process;
   QThread *updateThread;
   QVector<RFIOChannel *> channel_list;

   int channel;

   void init();
};

#endif // RFIO_H
