#ifndef SERVOOBJECT_H
#define SERVOOBJECT_H

#include "ArduinoOutputObject.h"
#include "libs/ServoESP.h"

class ServoObject : public ArduinoOutputObject
{
public:
    ServoObject(const int& pin);

    void rotate(const int& angle);

    int angle() const { return m_angle; }

private:
    ServoESP m_servo;
    int m_angle;
};

#endif