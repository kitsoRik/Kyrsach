#include "ArduinoOutputObject.h"

ArduinoOutputObject::ArduinoOutputObject(const int &pin) : ArduinoObject(pin)
{
    pinMode(this->pin(), OUTPUT);
}