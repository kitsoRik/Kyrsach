#ifndef ARDUINO_OBJECT_H
#define ARDUINO_OBJECT_H

#include <Arduino.h>
#include <string>

class ArduinoObject
{
public:
    enum Type
    {
        Rele,
        Servo,
        Piezo,
        Led,
        MagSig
    };
    explicit ArduinoObject(const int& pin);

    virtual void turnOn();
    virtual void turnOff();
    inline virtual void turn() { if(m_value) turnOff(); else turnOn(); }

    inline int pin() const { return m_pin; }
    inline void setPin(const int& pin) { m_pin = pin; }

    inline virtual bool value() const { return m_value; }
    inline void setValue (const bool &value) { m_value = value; }

    inline Type type() const { return m_type; }
    inline void setType(const int& type) { m_type = static_cast<Type>(type); }

    inline std::string name() const { return m_name; }
    inline void setName(const std::string &name) { m_name = name; }

private:
    int m_pin;
    bool m_value;
    Type m_type;
    std::string m_name;
    
};

#endif