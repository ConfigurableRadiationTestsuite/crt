#ifndef RFIO_H
#define RFIO_H

/*
* Author: Mattis Jaksch
*
* Presents an RF device with multiple channels;
* data is received via netcat, distributed to the channels
* and eventually evaluated
*
*/

class QProcess;

#include "Component.h"
#include "RFIOChannel.h"

constexpr int BYTE_PER_SAMPLE = 2;
constexpr int BYTE_PER_CHANNEL = 4;

class RFIO : public Component {
Q_OBJECT

public:
   RFIO(RunManager * runManager, const QString &config);
   RFIO(RunManager * runManager, const QString &m_element_name, const QString &address, int channel);
   virtual ~RFIO() override;

   QVector<RFIOChannel *> get_channel_list() const {return channel_list;}

   void set_config() override;

public slots:
   void init() override;
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
   int channel;

   QProcess *process;
   QVector<RFIOChannel *> channel_list;


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
