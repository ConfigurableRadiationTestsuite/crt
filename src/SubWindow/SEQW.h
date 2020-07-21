#ifndef SEQW_H
#define SEQW_H

class Sequencer;

#include "SubWindow.h"

class SEQW : public SubWindow {
public:
    SEQW(RunManager *m_runManager, Sequencer *sequencer);
    virtual ~SEQW() override;

private:
    Sequencer *seq;

    void create_layout() override;
};

#endif // SEQW_H
