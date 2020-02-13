#include "RFIOChannel.h"

#include "src/Manager/RunManager.h"

#include <QtMath>

RFIOChannel::RFIOChannel(int number, int buffersize) : number(number), buffersize(buffersize) {}

RFIOChannel::~RFIOChannel() {}

void RFIOChannel::update_data(){
    if(is_eval) {
        evaluate_data(i_data);
        evaluate_data(q_data);
    }
    else
        is_evaluable();

    set_data(i_data, i_plot_data);
    set_data(q_data, q_plot_data);

    emit channel_updated();
}

int RFIOChannel::get_period(const QVector<int> &data) {
    int period = 0, cnt = 0, samples = 0;
    bool sign = data[0] > 0 ? true : false;

    for(QVector<int>::const_iterator it = data.begin(); it != data.end(); it++) {
        //3 Periods should be enough
        if(cnt > 5)
            break;

        //Ignore values near zero
        if(-qPow(2, BITS_TO_IGNORE) < *it || *it < qPow(2, BITS_TO_IGNORE))
            continue;

        //Check if the sign has changed
        if(sign != (*it > 0 ? true : false)) {
            period += samples;
            samples = 0;
            cnt++;
            sign = !sign;
        }
        samples++;
    }

    if(cnt == 0) {
        set_evaluation(0);
        return 0;
    }

    return period / cnt;
}

int RFIOChannel::get_zero(const QVector<int> &data) {
    bool capture = false;

    for(int i = 0; i < data.size(); i++) {
        if(!capture && data[i] < -qPow(2, BITS_TO_IGNORE))
            continue;

        if(qPow(2, BITS_TO_IGNORE) < data[i])
            return i;
    }

    return 0;
}

bool RFIOChannel::is_evaluable() {

}

void RFIOChannel::set_margin(int scale) {

}

void RFIOChannel::set_evaluation(int eval) {
    is_eval = eval > 0 ? true : false;

    emit evaluable_changed(eval);
}

bool RFIOChannel::evaluate_data(const QVector<int> &data) {

}

void RFIOChannel::set_data(const QVector<int> &input, QVector<int> &output) {
    int period = get_period(input);
    int zero = get_zero(input);

    for(int i = zero; i < input.size(); i++) {
        if(period*4 < i)
            return ;
        output.push_back(input[i]);
    }
}
