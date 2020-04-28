#include "RFIOChannel.h"

#include "src/Manager/RunManager.h"

#include <QtMath>

RFIOChannel::RFIOChannel(RunManager *runManager, const QString &element_name, int number, int margin)
    : runManager(runManager), element_name(element_name), number(number), margin(margin) {

    data_valid = false;
    data_analyze = false;
}

RFIOChannel::~RFIOChannel() {}

void RFIOChannel::set_data_analyze(int analyze) {
    if(data_valid)
        data_analyze = analyze > 0 ? true : false;
}

void RFIOChannel::analyze_data() {
    /* Create plot data */
    clear_data(i_plot_data);
    int start = get_zero(i_data);
    generate_plot_data(start, i_data, i_plot_data);
    clear_data(q_plot_data);
    generate_plot_data(start, q_data, q_plot_data);

    /* Generate ref_data */
    if(margin_changed && data_valid) {
        generate_ref_data(i_data);
        margin_changed = false;
    }

    /* Huge evaluation */
    if(data_valid && data_analyze) {
        bool ok = true;
        ok &= evaluate_data(i_data);
        ok &= evaluate_data(q_data);

        if(!ok)
            emit error(i_data, q_data, number);
    }

    /* Data valid check */
    if(!data_valid && get_data_valid(i_data) && get_data_valid(q_data)) {
        generate_ref_data(i_data);
        data_valid = true;
    }

    /* Delete data */
    clear_data(i_data);
    clear_data(q_data);

    emit announce_data_valid(!data_valid);

    emit finished();
}

int RFIOChannel::get_zero(const QVector<int> &data) {
    int minimum = 0;
    int maximum = 0;
    int min_point = 0;
    int max_point = 0;

    /* Search minimum */
    for(int i = 0; i < data.size(); i++) {

        if(minimum == 0 && data[i] > -qPow(2, BITS_TO_IGNORE))
            continue;

        if(data[i] < minimum) {
            minimum = data[i];
            min_point = i;
            continue;
        }

        if(data[i] > 0) {
            break;
        }
    }

    /* Search maximum */
    for(int i = min_point; i < data.size(); i++) {

        if(maximum == 0 && data[i] < qPow(2, BITS_TO_IGNORE))
            continue;

        if(data[i] > maximum) {
            maximum = data[i];
            max_point = i;
            continue;
        }

        if(data[i] < 0) {
            break;
        }
    }

    int zero_point = min_point + (max_point - min_point) / 2;

    /* Check one point before / after */
    if(qAbs(data[position(zero_point-1, data)]) < qAbs(data[zero_point]))
        return zero_point-1;

    if(qAbs(data[position(zero_point+1, data)]) < qAbs(data[zero_point]))
        return zero_point+1;

    return zero_point;
}

bool RFIOChannel::get_data_valid(const QVector<int> &data) {
    bool ok = true;

    /* Check if there is possibly valid data */
    ok &= MIN_DATAPOINTS < get_period(data);
    ok &= get_maximum(data) > qPow(2, BITS_TO_IGNORE);
    ok &= get_minimum(data) < - qPow(2, BITS_TO_IGNORE);

    return ok;
}

int RFIOChannel::get_period(const QVector<int> &data) {
    int period = 0, cnt = 0, samples = 0;
    bool sign = true;

    for(int i = get_zero(data); i < data.size(); i++) {
        //10 Half-Periods should be enough
        if(cnt > 9)
            break;

        //Ignore values near zero
        if(-qPow(2, BITS_TO_IGNORE) < data[i] && data[i] < qPow(2, BITS_TO_IGNORE)) {
            samples++;
            continue;
        }

        //Check if the sign has changed
        if(sign != (data[i] > 0 ? true : false)) {
            period += samples;
            samples = 0;
            cnt++;
            sign = !sign;
        }
        samples++;
    }

    if(cnt <= 1)
        return 0;

    return 2 * period / cnt;
}

bool RFIOChannel::evaluate_data(const QVector<int> &input) {
    int ref_i = 0;

    reset_transition();

    for(int i = get_zero(input); i < input.size(); i++) {
        if(is_transition(input[i])) {
            reset_transition();
            i = qAbs(input[position(i-1, input)]) < qAbs(input[i]) ? i-1 : i;
            ref_i = 0;
        }

        if(ref_i >= ref_data.size())
            return false;

        if(input[i] < ref_data[ref_i].bottom || ref_data[ref_i].top < input[i])
            return false;

        ref_i++;
    }

    return true;
}

void RFIOChannel::generate_ref_data(const QVector<int> &input) {
    int period = get_period(input);
    int min = get_minimum(input);
    int max = get_maximum(input);
    int amplitude = (qAbs(min) + qAbs(max)) / 2;

    ref_data.clear();
    ref_data.reserve(period);

    for(int i = 0; i < period*1.5; i++) {
        int top = amplitude * qSin(float(i) / float(period) * 2 * M_PI) + margin;
        int bottom = amplitude * qSin(float(i) / float(period) * 2 * M_PI) - margin;
        ref_data.push_back(ReferencePoint{top, bottom});

        if(i >= period && ref_data[i].top < 0) {
            ref_data.pop_back();
            break;
        }
    }
}

void RFIOChannel::generate_plot_data(int start, const QVector<int> &input, QVector<double> &output) {
    int period = get_period(input);

    for(int i = start; i < input.size(); i++) {
        if((period*4 + start) < i)
            return ;
        output.push_back(input[i]);
    }
}

bool RFIOChannel::is_transition(int value) {
    if(!capture_transition && value < -qPow(2, BITS_TO_IGNORE))
        capture_transition = true;

    if(capture_transition && value > qPow(2, BITS_TO_IGNORE)) {
        capture_transition = false;
        return true;
    }

    return false;
}

void RFIOChannel::set_sample_position(long long position) {
    sample_position = position;
}

void RFIOChannel::set_margin(int margin) {
    this->margin = qPow(2, margin);
    margin_changed = true;

    emit announce_margin_changed("2^" + QString::number(margin));
}
