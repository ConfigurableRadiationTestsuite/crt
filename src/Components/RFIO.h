#ifndef RFIO_H
#define RFIO_H

/*
* Author: Mattis Jaksch
*
*/

class RFIOChannel;
class RFIOUpdater;

class QProcess;
class QThread;

#include "Component.h"

class RFIO : public Component {
Q_OBJECT

public:
   RFIO(RunManager * runManager, const QString &config);
   RFIO(RunManager * runManager, const QString &m_element_name, const QString &address, int channel);
   virtual ~RFIO() override;

   QVector<RFIOChannel *> get_channel_list() const {return channel_list;}

   void set_config() override;

public slots:
   void update() override {}

   void set_single_shot();
   void set_multi_shot();

private slots:
   void reconnect();
   void handle_error(QVector<int> i_data, QVector<int> q_data, int number);
   void set_thread_destroyed();

private:
   QString address;
   int port;
   bool is_single_shot = false;
   bool is_destroyed;

   RFIOUpdater *rfioUpdater;
   QProcess *process;
   QThread *updateThread;
   QVector<RFIOChannel *> channel_list;

   int channel;

   void init();

   QVector<QString> generate_header() override;
};

#endif // RFIO_H
