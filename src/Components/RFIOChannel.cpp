#include "RFIOChannel.h"

#include "src/Manager/RunManager.h"

#include <QtMath>

RFIOChannel::RFIOChannel(int channel_number) : channel_number(channel_number) {}

RFIOChannel::~RFIOChannel() {}

void RFIOChannel::update_data(){
    if(is_eval) {
        evaluate_data(i_data);
        evaluate_data(q_data);
    }
    else
        check_data_valid();

    generate_plot_data(i_data, i_plot_data);
    generate_plot_data(q_data, q_plot_data);

    emit channel_updated();
}

int RFIOChannel::get_period(const QVector<int> &data) {
    int period = 0, cnt = 0, samples = 0;
    bool sign = data[0] > 0 ? true : false;

    for(QVector<int>::const_iterator it = data.begin(); it != data.end(); it++) {
        //6 Half-Periods should be enough
        if(cnt > 6)
            break;

        //Ignore values near zero
        if(-qPow(2, BITS_TO_IGNORE) < *it && *it < qPow(2, BITS_TO_IGNORE))
            continue;

        //Check if the sign has changed
        if(sign != (*it > 0 ? true : false)) {
            if(cnt != 0) //Ignore the first transition
                period += samples;
            samples = 0;
            cnt++;
            sign = !sign;
        }
        samples++;
    }

    if(cnt <= 1)
        return 0;

    return 2 * period / (cnt - 1);
}

int RFIOChannel::get_zero(const QVector<int> &data) {
    bool capture = false;

    for(int i = 0; i < data.size(); i++) {
        if(data[i] < -qPow(2, BITS_TO_IGNORE))
            capture = true;

        if(capture && qPow(2, BITS_TO_IGNORE) < data[i])
            return i;
    }

    return 0;
}

void RFIOChannel::check_data_valid() {
    bool ok = true;

    /* Check if there is possibly valid data */
    ok &= MIN_DATAPOINTS < get_period(i_data);
    ok &= MIN_DATAPOINTS < get_period(q_data);

    ok &= get_maximum(i_data) > qPow(2, BITS_TO_IGNORE);
    ok &= get_maximum(q_data) > qPow(2, BITS_TO_IGNORE);

    ok &= get_minimum(i_data) < - qPow(2, BITS_TO_IGNORE);
    ok &= get_minimum(q_data) < - qPow(2, BITS_TO_IGNORE);

    /* Set data valid */
    if(ok) {
        data_valid = true;
        emit has_data(true);
    }
    else {
        data_valid = false;
        emit has_data(false);
    }
}

void RFIOChannel::set_margin(int scale) {
    assert(0 < scale && scale < 10);
    margin = qPow(2, scale);
}

void RFIOChannel::set_evaluation(int eval) {
    is_eval = eval > 0 ? true : false;
}

void RFIOChannel::evaluate_data(const QVector<int> &data) {
    //Find the first zero

    //Compare all values till the next zero

    //Check if the period is ok

    //emit signal

}

void RFIOChannel::generate_plot_data(const QVector<int> &input, QVector<double> &output) {
    int period = get_period(input);
    int zero = get_zero(input);

    for(int i = zero; i < input.size(); i++) {
        if((period*4 + zero) < i)
            return ;
        output.push_back(input[i]);
    }
}

void RFIOChannel::clear_data() {
    int buffersize = i_data.size();

    i_data.clear(); i_data.reserve(buffersize);
    q_data.clear(); q_data.reserve(buffersize);
    i_plot_data.clear(); i_plot_data.reserve(buffersize);
    q_plot_data.clear(); q_plot_data.reserve(buffersize);
}
