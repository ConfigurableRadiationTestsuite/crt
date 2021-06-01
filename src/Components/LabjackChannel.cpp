#include "LabjackChannel.h"

#include <QElapsedTimer>
#include <QtMath>

LabjackChannel::LabjackChannel(QString const& name,
                               int* handle,
                               int p_chan,
                               int n_chan,
                               int gain,
                               double boundary)
    : name(name),
      handle(handle),
      p_chan(p_chan),
      n_chan(n_chan),
      boundary(boundary),
      external_gain(gain)
{
    this->is_input = true;
    this->is_differential = n_chan == 199 ? false : true;

    set_differential();
}

LabjackChannel::~LabjackChannel() {}

void LabjackChannel::update()
{
    read(get_pchan_address(), LJM_FLOAT32, value);
    set_range();

    check_boundary();
    emit value_changed(QString::number(get_value()));
}

bool LabjackChannel::check_boundary()
{
    bool ok = true;

    if(boundary != 0 && qFabs(boundary) < qFabs(get_value()))
    {
        emit boundary_check_failed();
        ok &= false;
    }

    return ok;
}

void LabjackChannel::set_range()
{
    foreach (double available_range, range_list)
    {
        if(value < 0.9 * available_range)
        {
            if(range == available_range)
            {
                return ;
            }
            else
            {
                //Write to address
                range = available_range;
                write(get_pchan_range_address(), LJM_FLOAT32, range);
                write(get_nchan_range_address(), LJM_FLOAT32, range);
                return ;
            }
        }
    }
}
