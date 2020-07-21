#include "SEQW.h"

#include "../Components/Sequencer.h"

SEQW::SEQW(RunManager *m_runManager, Sequencer *seq)
    : SubWindow(m_runManager, seq), seq(seq) {

}

SEQW::~SEQW() {
    delete seq;
}

void SEQW::create_layout() {

}
