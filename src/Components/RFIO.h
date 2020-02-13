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

#include "src/Configuration/ConfigElement.h"

#include <QWidget>

class RFIO : public QWidget, public ConfigElement {
Q_OBJECT

public:
   RFIO(RunManager * runManager, const QString &config);
   RFIO(RunManager * runManager, const QString &address, int channel);
   virtual ~RFIO() override;

   QVector<RFIOChannel *> get_channel_list() const {return channel_list;}

public slots:
   void update_channel();
   //void start();
   //void stop();

private:
   RunManager *runManager;
   QString address;

   QTimer *log_timer;

   int buffersize = 4096;
   QVector<RFIOChannel *> channel_list;

   bool connect_to_device();

   int create_2b_number(char msb, char lsb);

   void start_logging();
   void stop_logging();
   void init();
};

#endif // RFIO_H
