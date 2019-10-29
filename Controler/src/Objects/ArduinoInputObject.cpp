#include "ArduinoInputObject.h"

ArduinoInputObject::ArduinoInputObject(const int& pin) : ArduinoObject(pin)
{
    pinMode(this->pin(), INPUT);
}