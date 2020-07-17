#ifndef RFIO_H
#define RFIO_H

/*
* Author: Mattis Jaksch
*
*/

#define BYTE_PER_SAMPLE 2
#define BYTE_PER_CHANNEL 4

class QProcess;
class QThread;

#include "RFIOChannel.h"

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
   void update() override;

   void set_single_shot();
   void set_multi_shot();

private slots:
   void handle_error(QVector<IQSample> data, int number);

private:
   QString address;
   int port;
   bool is_single_shot = false;
   bool is_destroyed;

   QProcess *process;
   QVector<RFIOChannel *> channel_list;

   int channel;

   void init();

   QStringList generate_header() override;

   QByteArray dummy_iq(int period, int channel);
};

inline void RFIO::set_single_shot() {
    is_single_shot = true;
}

inline void RFIO::set_multi_shot() {
    is_single_shot = false;
}

#endif // RFIO_H
