#include "IQVector.h"

QPair<int, int> IQVector::get_zero() const
{
    QPair<int, int> minimum(0, 0);
    QPair<int, int> min_point(0, 0);
    QPair<int, int> zero_point(0, 0);

    /* Search minimum */
    for(int i = 0; i < size(); i++)
    {
        if(!minimum_search(minimum.first, min_point.first, at(i).get_i(), i))
        {
            break;
        }
    }

    for(int i = 0; i < size(); i++)
    {
        if(!minimum_search(minimum.second, min_point.second, at(i).get_q(), i))
        {
            break;
        }
    }

    /* Search zero */
    for(int i = min_point.first; i < size(); i++)
    {
        if(at(i).get_i() > 0)
        {
            break;
        }

        zero_point.first = i;
    }

    for(int i = min_point.second; i < size(); i++)
    {
        if(at(i).get_q() > 0)
        {
            break;
        }

        zero_point.second = i;
    }

    /* Check one point before / after */
    if(qAbs(at(position(zero_point.first-1)).get_i()) < qAbs(at(zero_point.first).get_i()))
    {
        zero_point.first -= 1;
    }

    if(qAbs(at(position(zero_point.second-1)).get_q()) < qAbs(at(zero_point.second).get_q()))
    {
        zero_point.second -= 1;
    }

    if(qAbs(at(position(zero_point.first+1)).get_i()) < qAbs(at(zero_point.first).get_i()))
    {
        zero_point.first += 1;
    }

    if(qAbs(at(position(zero_point.second+1)).get_q()) < qAbs(at(zero_point.second).get_q()))
    {
        zero_point.second += 1;
    }

    return zero_point;
}

bool IQVector::is_valid() const
{
    bool ok = true;

    /* Check if there is possibly valid data */
    QPair<int, int> period = get_period();

    ok &= MIN_DATAPOINTS < period.first;
    ok &= MIN_DATAPOINTS < period.second;

    ok &= get_maximum(get_i().mid(0, period.first*2)) > qPow(2, BITS_TO_IGNORE);
    ok &= get_maximum(get_q().mid(0, period.second*2)) > qPow(2, BITS_TO_IGNORE);

    ok &= get_minimum(get_i().mid(0, period.first*2)) < -qPow(2, BITS_TO_IGNORE);
    ok &= get_minimum(get_q().mid(0, period.second*2)) < -qPow(2, BITS_TO_IGNORE);

    return ok;
}

QPair<int, int> IQVector::get_period() const
{
    QPair<int, int> period(0,0), cnt(0,0), samples(0,0);
    QPair<bool, bool> sign(true, true);

    for(int i = get_zero().first; i < size(); i++)
    {
        if(!period_search(cnt.first, samples.first, period.first, sign.first, at(i).get_i()))
        {
            break;
        }
    }

    for(int i = get_zero().second; i < size(); i++)
    {
        if(!period_search(cnt.second, samples.second, period.second, sign.second, at(i).get_i()))
        {
            break;
        }
    }

    if(cnt.first <= 1)
    {
        period.first = 0;
        cnt.first = 1;
    }

    if(cnt.second <= 1)
    {
        period.second = 0;
        cnt.second = 1;
    }

    period.first = (2 * period.first) / cnt.first;
    period.second = (2 * period.second) / cnt.second;

    return period;
}
