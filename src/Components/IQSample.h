#ifndef IQSAMPLE_H
#define IQSAMPLE_H

#include <QByteArray>

class IQSample {
public:
    IQSample() : I(0), Q(0) {}

    template<typename T>
    IQSample(T I, T Q)
        : I(I), Q(Q) {}

    template<typename T>
    IQSample(T IQ)
        : I(IQ), Q(IQ) {}

    IQSample(const QByteArray &arr) {
        /* First half I */
        I = get_iq(arr.left(arr.size()/2));

        /* Second half Q */
        Q = get_iq(arr.right(arr.size()/2));
    }

    int get_i() const {return I;}
    int get_q() const {return Q;}

    IQSample operator+(const IQSample&);
    IQSample operator-(const IQSample&);
    IQSample operator*(const IQSample&);

private:
    int I, Q;

    int get_iq(const QByteArray &arr);
};

inline IQSample IQSample::operator+(const IQSample &a) {
    this->I += a.I;
    this->Q += a.Q;
    return *this;
}
inline IQSample IQSample::operator-(const IQSample &a) {
    this->I -= a.I;
    this->Q -= a.Q;
    return *this;
}

inline IQSample IQSample::operator*(const IQSample &a) {
    this->I *= a.I;
    this->Q *= a.Q;
    return *this;
}

inline bool operator< (const IQSample& a, const IQSample& b) {
    if((a.get_i() < b.get_i() && a.get_i() < b.get_q()) ||
            (a.get_q() < b.get_i() && a.get_q() < b.get_i()))
        return true;

    return false;
}

inline int IQSample::get_iq(const QByteArray &arr) {
    int result = static_cast<int8_t>(arr.back());

    for(QByteArray::const_iterator it = arr.end() - 1; it != arr.begin(); --it) {
        result <<= 8;
        result += static_cast<uint8_t>(*it);
    }

    return result;
}


#endif // IQSAMPLE_H
