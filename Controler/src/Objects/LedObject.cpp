#include "LedObject.h"

LedObject::LedObject(const int& pin, const bool &value)
    : ArduinoOutputObject(pin)
{
    if(value)
        this->turnOn();
    else
        this->turnOff();   
    setType(Led);
}