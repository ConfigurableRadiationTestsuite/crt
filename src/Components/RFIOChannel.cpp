#include "RFIOChannel.h"

#include "src/Manager/RunManager.h"

#include <QtMath>

RFIOChannel::RFIOChannel(RunManager *runManager, const QString &element_name, int number, int margin)
    : runManager(runManager), element_name(element_name), number(number), margin(margin) {

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
            emit error();
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

void RFIOChannel::handle_error() {
    /* Write the raw data to the runManager */
    runManager->register_component(this, element_name + QString::number(number));

    runManager->set_file_header(this, {"Sample", "i", "q"});

    for(int i = 0; i < i_data.size(); i++)
        runManager->append_values_to_file(this, {double(i), double(i_data[i]), double(q_data[i])});

    runManager->deregister_component(this);
}

int RFIOChannel::get_zero(const QVector<int> &data) {
    bool capture = false;

    for(int i = 0; i < data.size(); i++) {
        /* Wait till the curve is below zero and the noiseloor */
        if(data[i] < -qPow(2, BITS_TO_IGNORE))
            capture = true;

        /* Capture the translation from zero beyond the noisefloor */
        if(capture && qPow(2, BITS_TO_IGNORE) < data[i])
            return qAbs(data[position(i-1, data)]) < qAbs(data[i]) ? i-1 : i;
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
    bool sign = true;

    for(int i = get_zero(data); i < data.size(); i++) {
        //6 Half-Periods should be enough
        if(cnt > 5)
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
}
