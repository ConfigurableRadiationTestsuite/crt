#ifndef RFIOCHANNEL_H
#define RFIOCHANNEL_H

/*
* Author: Mattis Jaksch
*
*/

class RunManager;

#include "IQVector.h"

#include <QObject>
#include <QtMath>

class RFIOChannel : public QObject {
Q_OBJECT

public:
   RFIOChannel(int number, int margin=0)
       : number(number), margin(margin) {}

   IQVector get_raw_data() const {return raw_data;}
   IQVector get_ref_data_high() const {return ref_data_high;}
   IQVector get_ref_data_low() const {return ref_data_low;}
   IQVector get_plot_data() const {return plot_data;}

   long long get_sample_position() const {return sample_position;}
   int get_channel_number() const {return number;}
   int get_margin() const {return margin;}

   void set_sample_position(long long position);

   bool is_data_valid() const {return data_valid;}
   bool is_data_analyze() const {return data_analyze;}

   void append_value(IQSample sample) {raw_data.push_back(sample);}
   void analyze_data();

   template<typename T>
   void clear_data(QVector<T> &data);

public slots:
   void set_data_analyze(int );
   void set_margin(int );

signals:
   void error(QVector<IQSample> data, int channel);
   void announce_data_valid(bool);
   void finished();
   void announce_margin_changed(const QString &text);

private:
   int number;
   int margin;

   bool data_valid=false, data_analyze=false;
   bool capture_transition = false;
   bool margin_changed = false;
   long long sample_position;

   IQVector raw_data, plot_data;
   IQVector ref_data_high, ref_data_low;

   void generate_plot_data(QPair<int, int> start, const IQVector &input, IQVector &output);
   void generate_ref_data(const IQVector &input);

   bool evaluate_vector(const IQVector &input);
   int evaluate_sample(int value, int low, int high);

   void reset_transition() {capture_transition = false;}
   bool is_transition(int value);
};

template<typename T>
inline void RFIOChannel::clear_data(QVector<T> &data) {
    int buffersize = data.size();
    data.clear();
    data.reserve(buffersize);
}

inline void RFIOChannel::set_sample_position(long long position) {
    sample_position = position;
}

inline void RFIOChannel::set_data_analyze(int analyze) {
    if(data_valid)
        data_analyze = analyze > 0 ? true : false;
}

inline bool RFIOChannel::is_transition(int value) {
    if(!capture_transition && value < -qPow(2, BITS_TO_IGNORE))
        capture_transition = true;

    if(capture_transition && value > qPow(2, BITS_TO_IGNORE)) {
        capture_transition = false;
        return true;
    }

    return false;
}

inline int RFIOChannel::evaluate_sample(int value, int low, int high) {
    if(is_transition(value))
        return 1;

    if(value < low || high < value)
        return -1;

    return 0;
}

#endif // RFIOCHANNEL_H
