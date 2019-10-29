#include "ServoObject.h"

ServoObject::ServoObject(const int& pin) : ArduinoOutputObject(pin)
{
    m_servo.attach(this->pin());

    Serial.println("Servo added");
    setType(ArduinoObject::Servo);
}

void ServoObject::rotate(const int& angle)
{
    m_angle = angle < 0 ? 0 : angle > 180 ? 180 : angle;
    m_servo.write(m_angle);
}