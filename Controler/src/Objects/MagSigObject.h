#ifndef MAGSIGOBJECT_H
#define MAGSIGOBJECT_H

#include "ArduinoInputPullupObject.h"

class MagSigObject : public ArduinoInputPullupObject
{
public:
    MagSigObject(const int& pin);
};

#endif