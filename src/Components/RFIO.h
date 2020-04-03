#ifndef RFIO_H
#define RFIO_H

/*
* Author: Mattis Jaksch
*
*/

#define BYTE_PER_CHANNEL 4
#define BYTE_PER_SAMPLE 2

class RFIOChannel;
class RunManager;

class QProcess;

#include "src/Configuration/ConfigElement.h"

#include <QWidget>

class RFIO : public QWidget, public ConfigElement {
Q_OBJECT

public:
   RFIO(RunManager * runManager, const QString &config);
   RFIO(RunManager * runManager, const QString &address, int channel);
   virtual ~RFIO() override;

   QVector<RFIOChannel *> get_channel_list() const {return channel_list;}

   void set_config() override;

public slots:
   void update_device();

   void start_logging();
   void stop_logging();

private:
   RunManager *runManager;
   QString address;
   QProcess *process;

   QTimer *log_timer;

   QVector<RFIOChannel *> channel_list;

   bool connect_device();
   void disconnect_device();

   int create_2b_number(char lsb, char msb);

   void init();
};

#endif // RFIO_H
