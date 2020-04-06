#ifndef RFIOCHANNEL_H
#define RFIOCHANNEL_H

/*
* Author: Mattis Jaksch
*
*/

#define MIN_DATAPOINTS 10
#define BITS_TO_IGNORE 3

#include <QObject>

class RFIOChannel : public QObject {
Q_OBJECT

public:
   RFIOChannel(int channel_number);
   virtual ~RFIOChannel() override;

   QVector<int> get_i_data() const {return i_data;}
   QVector<int> get_q_data() const {return q_data;}
   QVector<double> get_i_plot_data() const {return i_plot_data;}
   QVector<double> get_q_plot_data() const {return q_plot_data;}

   int get_channel_number() const {return channel_number;}
   int get_period(const QVector<int> &data);
   int get_zero(const QVector<int> &data);
   int get_minimum(const QVector<int> &data);
   int get_maximum(const QVector<int> &data);
   long long get_sample_position() const {return sample_position;}

   void set_sample_position(long long position) {sample_position = position;}
   void append_values(int i_value, int q_value);

   bool is_valid() const {return data_valid;}

   void clear_data();

public slots:
   void set_margin(int scale);
   void set_evaluation(int eval);

   void update_data();

signals:
   void iq_error(const QVector<double> &data);
   void has_data(bool data);
   void channel_updated();

private:
   int channel_number;
   int margin;
   bool is_eval = false;
   bool data_valid = false;
   QVector<int> i_data;
   QVector<int> q_data;
   QVector<double> i_plot_data;
   QVector<double> q_plot_data;

   long long sample_position;

   void evaluate_data(const QVector<int> &data);
   void generate_plot_data(const QVector<int> &input, QVector<double> &output);

    void check_data_valid();
};

inline int RFIOChannel::get_minimum(const QVector<int> &data) {
    return *std::min_element(data.begin(), data.end());
}

inline int RFIOChannel::get_maximum(const QVector<int> &data) {
    return *std::max_element(data.begin(), data.end());
}

inline void RFIOChannel::append_values(int i_value, int q_value) {
    i_data.push_back(i_value);
    q_data.push_back(q_value);
}

#endif // RFIOCHANNEL_H
