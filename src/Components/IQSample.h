#ifndef IQSAMPLE_H
#define IQSAMPLE_H

#include <QByteArray>

class IQSample
{
public:
    IQSample()
        : I(0), Q(0)
    {}

    template<typename T>
    IQSample(T I, T Q)
        : I(I), Q(Q)
    {}

    template<typename T>
    IQSample(T IQ)
        : I(IQ), Q(IQ)
    {}

    IQSample(const QByteArray& arr)
    {
        /* First half I */
        I = get_iq(arr.left(arr.size()/2));

        /* Second half Q */
        Q = get_iq(arr.right(arr.size()/2));
    }

    int get_i() const
    {
        return I;
    }

    int get_q() const
    {
        return Q;
    }

    IQSample operator+(const IQSample& a)
    {
        this->I += a.I;
        this->Q += a.Q;
        return *this;
    }

    IQSample operator-(const IQSample& a)
    {
        this->I -= a.I;
        this->Q -= a.Q;
        return *this;
    }

    IQSample operator*(const IQSample& a)
    {
        this->I *= a.I;
        this->Q *= a.Q;
        return *this;
    }

private:
    int I, Q;

    int get_iq(const QByteArray& arr)
    {
        int result = static_cast<int8_t>(arr.back());

        for(int i = arr.size()-2; i >= 0; --i)
        {
            result <<= 8;
            result += static_cast<uint8_t>(arr[i]);
        }

        return result;
    }
};

inline bool operator< (const IQSample& a, const IQSample& b)
{
    if((a.get_i() < b.get_i() && a.get_i() < b.get_q())
        || (a.get_q() < b.get_i() && a.get_q() < b.get_i()))
    {
        return true;
    }

    return false;
}

#endif // IQSAMPLE_H
