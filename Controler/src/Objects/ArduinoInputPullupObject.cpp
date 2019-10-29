#include "ArduinoInputPullupObject.h"

ArduinoInputPullupObject::ArduinoInputPullupObject(const int& pin)
    : ArduinoObject(pin)
{
    pinMode(pin, INPUT_PULLUP);
}

bool ArduinoInputPullupObject::value() const
{
    int pin = this->pin();
    bool value = !digitalRead(pin);
    return value;
}