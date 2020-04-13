#ifndef RFIOCHANNEL_H
#define RFIOCHANNEL_H

/*
* Author: Mattis Jaksch
*
*/

#define BITS_TO_IGNORE 5
#define MIN_DATAPOINTS 10

class RunManager;

#include <QObject>

struct ReferencePoint {
    int top;
    int bottom;
};

class RFIOChannel : public QObject {
Q_OBJECT

public:
   RFIOChannel(RunManager * runManager, const QString &element_name, int number, int margin=0);
   virtual ~RFIOChannel();

   QVector<int> get_i_data() const {return i_data;}
   QVector<int> get_q_data() const {return q_data;}
   QVector<double> get_i_plot_data() const {return i_plot_data;}
   QVector<double> get_q_plot_data() const {return q_plot_data;}

   int get_period(const QVector<int> &data);
   int get_zero(const QVector<int> &data);
   int get_minimum(const QVector<int> &data);
   int get_maximum(const QVector<int> &data);
   bool get_data_valid(const QVector<int> &data);

   long long get_sample_position() const {return sample_position;}
   int get_channel_number() const {return number;}
   int get_margin() const {return margin;}

   void set_sample_position(long long position);

   bool is_data_valid() const {return data_valid;}
   bool is_data_analyze() const {return data_analyze;}

   void append_value(int i, int q);
   void analyze_data();

   void clear_data(QVector<int> &data);
   void clear_data(QVector<double> &data);

public slots:
   void set_data_analyze(int );
   void set_margin(int );

private slots:
   void handle_error();

signals:
   void error();
   void announce_data_valid(bool);
   void finished();
   void announce_margin_changed(const QString &text);

private:
   RunManager *runManager;
   QString element_name;
   int number;
   int margin;

   bool data_valid, data_analyze;
   bool capture_transition = false;
   bool margin_changed = false;
   long long sample_position;

   QVector<int> i_data, q_data;
   QVector<ReferencePoint> ref_data;
   QVector<double> i_plot_data, q_plot_data;

   void generate_plot_data(int start, const QVector<int> &input, QVector<double> &output);
   void generate_ref_data(const QVector<int> &input);
   bool evaluate_data(const QVector<int> &input);

   void reset_transition();
   bool is_transition(int value);
   int position(int position, const QVector<int> &vec);
};

inline void RFIOChannel::reset_transition() {
    capture_transition = false;
}

inline int RFIOChannel::get_minimum(const QVector<int> &data) {
    return *std::min_element(data.begin(), data.end());
}

inline int RFIOChannel::get_maximum(const QVector<int> &data) {
    return *std::max_element(data.begin(), data.end());
}

inline void RFIOChannel::append_value(int i_value, int q_value) {
    i_data.push_back(i_value);
    q_data.push_back(q_value);
}

inline void RFIOChannel::clear_data(QVector<int> &data) {
    int buffersize = data.size();
    data.clear();
    data.reserve(buffersize);
}

inline void RFIOChannel::clear_data(QVector<double> &data) {
    int buffersize = data.size();
    data.clear();
    data.reserve(buffersize);
}

inline int RFIOChannel::position(int position, const QVector<int> &vec) {
    if(position < 0)
        return 0;
    if(position >= vec.size())
        return vec.size()-1;
    return position;
}
#endif // RFIOCHANNEL_H
