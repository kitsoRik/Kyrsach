#include "ArduinoObject.h"

ArduinoObject::ArduinoObject(const int &pin)
{
    m_pin = pin;
}

void ArduinoObject::turnOn()
{
    m_value = true;
    digitalWrite(m_pin, HIGH);
}

void ArduinoObject::turnOff()
{
    m_value = false;
    digitalWrite(m_pin, LOW);
}