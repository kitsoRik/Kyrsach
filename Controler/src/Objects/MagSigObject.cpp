#include "MagSigObject.h"

MagSigObject::MagSigObject(const int & pin)
    : ArduinoInputPullupObject(pin)
{
    setType(ArduinoObject::MagSig);
}