#ifndef TERMISTOROBJECT_H
#define TERMISTOROBJECT_H

#include "ArduinoInputObject.h"

class TermistorObject : public ArduinoInputObject
{
public:
    TermistorObject(const int& pin);

    int get() const;
};

#endif