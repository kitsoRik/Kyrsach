#ifndef LEDOBJECT_H
#define LEDOBJECT_H

#include "ArduinoOutputObject.h"

class LedObject : public ArduinoOutputObject
{
public:
    LedObject(const int &pin, const bool &value = false);
};

#endif
