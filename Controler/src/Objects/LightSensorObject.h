#ifndef LIGHTSENSOROBJECT_H
#define LIGHTSENSOROBJECT_H

#include <Arduino.h>
#include "ArduinoInputObject.h"

class LightSensorObject : public ArduinoInputObject
{
public:
    LightSensorObject(const int& pin);

    int get() const;
    float get01() const;
};

#endif