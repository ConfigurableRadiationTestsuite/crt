#ifndef IQVECTOR_H
#define IQVECTOR_H

#include "IQSample.h"

#include <QtMath>
#include <QVector>

constexpr int BITS_TO_IGNORE = 5;
constexpr int MIN_DATAPOINTS = 10;

class IQVector : public QVector<IQSample> {
public:

    QPair<int, int> get_period() const;
    QPair<int, int> get_zero() const;

    QPair<int, int> get_minimum() const;
    QPair<int, int> get_maximum() const;

    template<typename T>
    int get_minimum(const QVector<T> &input) const;
    template<typename T>
    int get_maximum(const QVector<T> &input) const;

    template<typename T>
    void get_i(QVector<T> &output) const;
    QVector<int> get_i() const;
    template<typename T>
    void get_q(QVector<T> &output) const;
    QVector<int> get_q() const;

    int position(int position) const;

    bool is_valid() const;

private:
    bool minimum_search(int &minimum, int &point, int value, int location) const;
    bool maximum_search(int &maximum, int &point, int value, int location) const;
    bool period_search(int &cnt, int &samples, int &period, bool &sign, int value) const;
};
template<typename T>
inline int IQVector::get_minimum(const QVector<T> &input) const {
    int min = *std::min_element(input.begin(), input.end());
    return min;
}

template<typename T>
inline int IQVector::get_maximum(const QVector<T> &input) const {
    return *std::max_element(input.begin(), input.end());
}

inline QPair<int, int> IQVector::get_minimum() const {
    IQSample min = *std::min_element(begin(), end());
    return QPair<int, int>(min.get_i(), min.get_q());
}

inline QPair<int, int> IQVector::get_maximum() const {
    IQSample max = *std::max_element(begin(), end());
    return QPair<int, int>(max.get_i(), max.get_q());
}

inline bool IQVector::minimum_search(int &minimum, int &point, int value, int location) const {
    if(minimum == 0 && value > -qPow(2, BITS_TO_IGNORE))
        return true;

    if(value < minimum) {
        minimum = value;
        point = location;
        return true;
    }

    if(value > 0)
        return false;

    return true;
}

inline bool IQVector::maximum_search(int &maximum, int &point, int value, int location) const {
    if(maximum == 0 && value < qPow(2, BITS_TO_IGNORE))
        return true;

    if(value > maximum) {
        maximum = value;
        point = location;
        return true;
    }

    if(value < 0)
        return false;

    return true;
}

inline bool IQVector::period_search(int &cnt, int &samples, int &period, bool &sign, int value) const {
    //10 Half-Periods should be enough
    if(cnt > 9)
        return false;

    //Ignore values near zero
    if(-qPow(2, BITS_TO_IGNORE) < value && value < qPow(2, BITS_TO_IGNORE)) {
        samples++;
        return true;
    }

    //Check if the sign has changed
    if(sign != (value > 0 ? true : false)) {
        period += samples;
        samples = 0;
        cnt++;
        sign = !sign;
    }

    samples++;

    return true;
}

inline int IQVector::position(int position) const {
    if(position < 0)
        return 0;

    if(position >= size())
        return size()-1;

    return position;
}

template<typename T>
inline void IQVector::get_i(QVector<T> &output) const {
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
        output.push_back((*it).get_i());
}

inline QVector<int> IQVector::get_i() const {
    QVector<int> output;
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
        output.push_back((*it).get_i());

    return output;
}

template<typename T>
inline void IQVector::get_q(QVector<T> &output) const {
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
        output.push_back((*it).get_q());
}

inline QVector<int> IQVector::get_q() const {
    QVector<int> output;
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
        output.push_back((*it).get_q());

    return output;
}
#endif // IQVECTOR_H
