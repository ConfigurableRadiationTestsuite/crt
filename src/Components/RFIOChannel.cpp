#include "RFIOChannel.h"

#include "src/Manager/RunManager.h"

void RFIOChannel::analyze_data()
{
    /* Create plot data */
    QPair<int, int> start = raw_data.get_zero();
    generate_plot_data(start, raw_data, plot_data);
    emit plot_data_changed(plot_data.get_i(), plot_data.get_q());

    /* Generate ref_data */
    if(margin_changed && data_valid)
    {
        generate_ref_data(raw_data);
        margin_changed = false;
    }

    /* Huge evaluation */
    if(data_valid && data_analyze)
    {
        bool ok = true;
        ok &= evaluate_vector(raw_data);

        if(!ok)
            emit error(raw_data, number);
    }

    /* Data valid check */
    if(!data_valid && raw_data.is_valid())
    {
        generate_ref_data(raw_data);
        data_valid = true;
    }

    /* Delete data */
    clear_data(raw_data);

    emit announce_data_valid(!data_valid);

    emit finished();
}

bool RFIOChannel::evaluate_vector(const IQVector& input)
{
    int ref_i = 0;
    int anomaly_counter = 0;
    QPair<int, int> period = input.get_period();

    reset_transition();

    for(int i = input.get_zero().first; i < input.size(); i++)
    {
        int res = evaluate_sample(input.at(i).get_i(), ref_data_low[ref_i].get_i(), ref_data_high[ref_i].get_i());

        /* Evaluate result */
        if(res == 1)
        {
            reset_transition();
            i = qAbs(input.position(i-1)) < qAbs(input[i].get_i()) ? i-1 : i;
            ref_i = 0;
            anomaly_counter = 0;
        }
        else if (res == 0)
        {
            anomaly_counter = 0;
        }
        else if (res == -1)
        {
            anomaly_counter++;
        }

        /* Check anomaly counter */
        if(anomaly_counter >= period.first / 2)
        {
            qDebug("Anomaly Encounter at I: " + (QString::number(i)).toLatin1());
            return false;
        }

        ref_i++;
    }

    reset_transition();

    ref_i = 0;
    anomaly_counter = 0;

    for(int i = input.get_zero().second; i < input.size(); i++)
    {
        int res = evaluate_sample(input.at(i).get_q(), ref_data_low[ref_i].get_q(), ref_data_high[ref_i].get_q());

        /* Evaluate result */
        if(res == 1)
        {
            reset_transition();
            i = qAbs(input.position(i-1)) < qAbs(input[i].get_q()) ? i-1 : i;
            ref_i = 0;
            anomaly_counter = 0;
        }
        else if (res == 0)
        {
            anomaly_counter = 0;
        }
        else if (res == -1)
        {
            anomaly_counter++;
        }

        /* Check anomaly counter */
        if(anomaly_counter == period.second / 2)
        {
            qDebug("Anomaly Encounter at Q: " + (QString::number(i)).toLatin1());
            return false;
        }

        ref_i++;
    }

    return true;
}

void RFIOChannel::generate_ref_data(const IQVector& input)
{
    QPair<int, int> period = input.get_period();
    int m_period = period.first < period.second ? period.second : period.first;

    QPair<int, int> maximum = input.get_maximum();
    QPair<int, int> minimum = input.get_minimum();

    QPair<int, int> amplitude;
    amplitude.first = (qAbs(maximum.first) + qAbs(minimum.first)) / 2;
    amplitude.second = (qAbs(maximum.second) + qAbs(minimum.second)) / 2;

    ref_data_low.resize(m_period*1.5 + 1);
    ref_data_high.resize(m_period*1.5 + 1);

    for(int i = 0; i < m_period*1.5; i++) {
        QPair<int, int> high;
        high.first = amplitude.first * qSin(float(i) / float(period.first) * 2 * M_PI) + margin;
        high.second = amplitude.second * qSin(float(i) / float(period.second) * 2 * M_PI) + margin;
        ref_data_high[i] = IQSample(high.first, high.second);

        QPair<int, int> low;
        low.first = amplitude.first * qSin(float(i) / float(period.first) * 2 * M_PI) - margin;
        low.second = amplitude.second * qSin(float(i) / float(period.second) * 2 * M_PI) - margin;
        ref_data_low[i] = IQSample(low.first, low.second);
    }
}

void RFIOChannel::generate_plot_data(QPair<int, int> start, const IQVector& input, IQVector& output)
{
    QPair<int, int> period = input.get_period();
    int m_period = period.first < period.second ? period.second : period.first;
    int m_start = (start.first < start.second ? start.first : start.second);

    output.resize(4 * m_period);

    for(int i = m_start; i < input.size(); i++) {
        if((4 * m_period + m_start) <= i)
            return ;
        output[i-m_start] = input[i];
    }
}

void RFIOChannel::set_margin(int margin)
{
    this->margin = qPow(2, margin);
    margin_changed = true;

    emit announce_margin_changed("2^" + QString::number(margin));
}
