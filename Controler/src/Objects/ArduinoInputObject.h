#ifndef ARDUINOINPUTOBJECT_H
#define ARDUINOINPUTOBJECT_H

#include "ArduinoObject.h"

class ArduinoInputObject : public ArduinoObject
{
public:
    ArduinoInputObject(const int& pin);
};

#endif