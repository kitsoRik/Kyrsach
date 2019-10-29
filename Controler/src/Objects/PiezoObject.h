#ifndef PIEZOOBJECT_H
#define PIEZOOBJECT_H

#include <Arduino.h>
#include "ArduinoOutputObject.h"
#include <list>

struct PiezoMelody
{
    int frequency;
    int duration;
};

class Controler;

class PiezoObject : public ArduinoOutputObject
{
public:
    PiezoObject(const int &channel, const int &pin);

    void play();
    void play(const std::list<PiezoMelody> &melodys);

    void stop();

    void turnOn() override { play(); }
    void turnOff() override { stop(); }
    void turn() override { if(m_playing) stop(); else play(); }

    bool playing() const { return m_playing; }
    bool stoped() const { return !m_playing; }

    inline int channel() const { return m_channel; }
    inline void setChannel(const int &channel) { m_channel = channel; }

private:
    int m_channel;
    bool m_playing;
};

#endif