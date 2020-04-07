#include "RFIOChannel.h"

#include <QtMath>

RFIOChannel::RFIOChannel(RunManager *runManager) : runManager(runManager) {

    data_valid = false;
    data_analyze = false;

    connect(this, SIGNAL(error()), this, SLOT(handle_error()));
}

RFIOChannel::~RFIOChannel() {}

void RFIOChannel::set_data_analyze(int analyze) {
    if(data_valid)
        data_analyze = analyze > 0 ? true : false;
}

void RFIOChannel::analyze_data() {
    /* Create plot data */
    generate_plot_data(i_data, i_plot_data);
    generate_plot_data(i_data, q_plot_data);

    /* Huge evaluation */
    if(data_valid && data_analyze) {
        emit error();
    }

    /* Data valid check */
    if(get_data_valid(i_data) && get_data_valid(q_data))
        data_valid = true;
    else
        data_valid = false;

    emit announce_data_valid(data_valid);

    emit finished();
}

void RFIOChannel::handle_error() {
    //Write the raw data to the runManager
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

    buffersize = i_plot_data.size();

    i_plot_data.clear(); i_plot_data.reserve(buffersize);
    q_plot_data.clear(); q_plot_data.reserve(buffersize);
}
