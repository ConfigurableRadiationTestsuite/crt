#ifndef IQVECTOR_H
#define IQVECTOR_H

#include "IQSample.h"

#include <QtMath>
#include <QVector>

static constexpr int BITS_TO_IGNORE = 3;
static constexpr int MIN_DATAPOINTS = 10;

class IQVector : public QVector<IQSample>
{
public:
    QPair<int, int> get_period() const;
    QPair<int, int> get_zero() const;

    QPair<int, int> get_minimum() const;
    QPair<int, int> get_maximum() const;

    template<typename T>
    int get_minimum(const QVector<T>& input) const;
    template<typename T>
    int get_maximum(const QVector<T>& input) const;

    template<typename T>
    void get_i(QVector<T>& output) const;
    QVector<int> get_i() const;
    template<typename T>
    void get_q(QVector<T>& output) const;
    QVector<int> get_q() const;

    int position(int position) const;

    bool is_valid() const;

private:
    bool minimum_search(int& minimum, int& point, int value, int location) const;
    bool maximum_search(int& maximum, int& point, int value, int location) const;
    bool period_search(int& cnt, int& samples, int& period, bool& sign, int value) const;
};

template<typename T>
inline int IQVector::get_minimum(const QVector<T>& input) const
{
    return *std::min_element(input.begin(), input.end());
}

template<typename T>
inline int IQVector::get_maximum(const QVector<T>& input) const
{
    return *std::max_element(input.begin(), input.end());
}

inline QPair<int, int> IQVector::get_minimum() const
{
    return QPair<int, int>(get_minimum(get_i()), get_minimum(get_q()));
}

inline QPair<int, int> IQVector::get_maximum() const
{
    return QPair<int, int>(get_maximum(get_i()), get_maximum(get_q()));
}

/* Search iteratively for the minimum by inserting a value
 * minimum - callback save for lowest value
 * point - callback save for lowest value position
 * value - current value
 * location - current position
 */
inline bool IQVector::minimum_search(int& minimum, int& point, int value, int location) const
{
    bool search = true;

    if(minimum == 0 && value > - qPow(2, BITS_TO_IGNORE))
    {
        search &= true;
    }
    else if(value < minimum)
    {
        minimum = value;
        point = location;
        search &= true;
    }
    else if(value > 0)
    {
        search &= false;
    }

    return search;
}

/* Search iteratively for the maximum by inserting a value
 * maximum - callback save for biggest value
 * point - callback save for biggest value position
 * value - current value
 * location - current position
 */
inline bool IQVector::maximum_search(int& maximum, int& point, int value, int location) const
{
    bool search = true;

    if(maximum == 0 && value < qPow(2, BITS_TO_IGNORE))
    {
        search &= true;
    }
    else if(value > maximum)
    {
        maximum = value;
        point = location;
        search &= true;
    }
    else if(value < 0)
    {
        search &= false;
    }

    return search;
}

/* Search periodicity by running over a vector
 * cnt - period counter
 * samples - current sample position
 * period - period result
 * sign - current sample sign
 * value - current sample value
 */
inline bool IQVector::period_search(int& cnt, int& samples,
                                    int& period, bool& sign, int value) const
{
    bool search = true;

    //10 Half-Periods should be enough
    if(cnt > 9)
    {
        search &= false;
    }
    else
    {
        //Ignore values near zero
        if(-qPow(2, BITS_TO_IGNORE) < value && value < qPow(2, BITS_TO_IGNORE))
        {
            samples++;
        }
        else
        {
            //Check if the sign has changed
            if(sign != (value > 0 ? true : false))
            {
                period += samples;
                samples = 0;
                cnt++;
                sign = !sign;
            }

            samples++;
        }
    }

    return search;
}

/* Get position with boundaries */
inline int IQVector::position(int position) const
{
    int result = position;

    if(position < 0)
    {
        result = 0;
    }
    else if(position >= size())
    {
        result = size()-1;
    }

    return result;
}

template<typename T>
inline void IQVector::get_i(QVector<T>& output) const
{
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
    {
        output.push_back((*it).get_i());
    }
}

inline QVector<int> IQVector::get_i() const
{
    QVector<int> output;
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
    {
        output.push_back((*it).get_i());
    }

    return output;
}

template<typename T>
inline void IQVector::get_q(QVector<T>& output) const
{
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
    {
        output.push_back((*it).get_q());
    }
}

inline QVector<int> IQVector::get_q() const
{
    QVector<int> output;
    output.reserve(size());

    for(IQVector::const_iterator it = begin(); it != end(); ++it)
    {
        output.push_back((*it).get_q());
    }

    return output;
}
#endif // IQVECTOR_H
