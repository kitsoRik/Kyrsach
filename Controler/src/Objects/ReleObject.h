#ifndef RELE_H
#define RELE_H

#include <WiFi.h>
#include <list>
#include "ArduinoOutputObject.h"

class ReleObject : public ArduinoOutputObject
{
public:
    explicit ReleObject(const int& pin);
};

#endif