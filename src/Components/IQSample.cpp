#include "IQSample.h"

#include <QByteArray>

IQSample::IQSample(const QByteArray &arr) {
    /* First half I */
    I = get_iq(arr.left(arr.size()/2));

    /* Second half Q */
    Q = get_iq(arr.right(arr.size()/2));
}

int IQSample::get_iq(const QByteArray &arr) {
    int result = static_cast<int8_t>(arr.back());

    for(QByteArray::const_iterator it = arr.end() - 1; it != arr.begin(); --it) {
        result <<= 8;
        result += static_cast<uint8_t>(*it);
    }

    return result;
}
