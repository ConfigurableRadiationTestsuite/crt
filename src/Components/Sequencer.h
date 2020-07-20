#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "Component.h"

class Sequencer : public Component {
public:
    Sequencer(RunManager *runManager, const QString &config);
};

#endif // SEQUENCER_H
