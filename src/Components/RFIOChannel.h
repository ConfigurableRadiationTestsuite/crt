#ifndef RFIOCHANNEL_H
#define RFIOCHANNEL_H

/*
* Author: Mattis Jaksch
*
*/

#define BITS_TO_IGNORE 3

#include <QWidget>

class RFIOChannel : public QWidget{
Q_OBJECT

public:
   RFIOChannel(int number, int buffersize);
   virtual ~RFIOChannel() override;

   QVector<int> get_i_data() const {return i_data;}
   QVector<int> get_q_data() const {return q_data;}

   int get_number() const {return number;}
   int get_period(const QVector<int> &data);
   int get_zero(const QVector<int> &data);
   int get_minimum(const QVector<int> &data);
   int get_maximum(const QVector<int> &data);
   long long get_sample_position() const {return sample_position;}

   void set_sample_position(long long position) {sample_position = position;}
   void append_values(int i_value, int q_value);

   void clear_data();

   bool is_evaluable();

public slots:
   void set_margin(int scale);
   void set_evaluation(int eval);

   void update_data();

signals:
   void iq_error(const QVector<double> &data);
   void evaluable_changed(int evaluable);
   void channel_updated();

private:
    bool is_eval = false;
    QVector<int> i_data;
    QVector<int> q_data;
    QVector<int> i_plot_data;
    QVector<int> q_plot_data;
    int number;
    int buffersize;

    QVector<double> reference_data;
    long long sample_position;

    bool evaluate_data(const QVector<int> &data);
    void set_data(const QVector<int> &input, QVector<int> &output);
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

inline void RFIOChannel::clear_data() {
    i_data.clear(); i_data.reserve(buffersize);
    q_data.clear(); q_data.reserve(buffersize);
    i_plot_data.clear(); i_plot_data.reserve(buffersize);
    q_plot_data.clear(); q_plot_data.reserve(buffersize);
}
#endif // RFIOCHANNEL_H
