#ifndef ARDUINOINPUTPULLUPOBJECT_H
#define ARDUINOINPUTPULLUPOBJECT_H

#include "ArduinoObject.h"

class ArduinoInputPullupObject : public ArduinoObject
{
public:
    ArduinoInputPullupObject(const int& pin);

    inline bool value() const override;
};

#endif