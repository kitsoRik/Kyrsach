#include "TermistorObject.h"

TermistorObject::TermistorObject(const int &pin) 
    : ArduinoInputObject(pin)
{
    
}

int TermistorObject::get() const 
{
    double value = analogRead(pin());
    //const int SERIESRESISTOR = 200;

    //value = (4095 / value) - 1;
    //value = SERIESRESISTOR / value;

    return value;
}