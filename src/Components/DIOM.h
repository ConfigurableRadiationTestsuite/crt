#ifndef DIOM_H
#define DIOM_H

/*
 * Author: Mattis Jaksch
 *
 * (Hardware-)Interface to a Digital
 * Input / Output Module
 *
 */

class DIOM {

public:
    DIOM();
    virtual ~DIOM();

private:
    void read_io();
    void write_io();

};

#endif // DIOM_H
