#ifndef IQSAMPLE_H
#define IQSAMPLE_H

class QByteArray;

class IQSample{
public:
    template<typename T>
    IQSample(T I, T Q)
        : I(I), Q(Q) {}

    template<typename T>
    IQSample(T IQ)
        : I(IQ), Q(IQ) {}

    IQSample(const QByteArray &arr);

    template<typename T>
    T get_i() const {return I;}
    template<typename T>
    T get_q() const {return Q;}

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


#endif // IQSAMPLE_H
