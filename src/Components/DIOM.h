#ifndef DIOM_H
#define DIOM_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface to a Digital
 * Input / Output Module
 *
 */

#include "src/Configuration/ConfigElement.h"

#include <QWidget>

class DIOM : public QWidget, public ConfigElement {
Q_OBJECT

public:
    DIOM();
    virtual ~DIOM() override;

private:
    void read_io();
    void write_io();

};

#endif // DIOM_H
