#include "LightSensorObject.h"

LightSensorObject::LightSensorObject(const int &pin) 
    : ArduinoInputObject (pin)
{

}

int LightSensorObject::get() const
{
    int value = analogRead(pin());
    return value;
}

float LightSensorObject::get01() const
{
    const float MAX_THRESHOLD = 1023;
    const float MIN_THRESHOLD = 0;
    
    int value = this->get();

    return value / 1023.0;
}