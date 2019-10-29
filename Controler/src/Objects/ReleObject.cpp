#include "ReleObject.h"

ReleObject::ReleObject(const int& pin) : ArduinoOutputObject(pin)
{
    setType(ArduinoObject::Rele);
}