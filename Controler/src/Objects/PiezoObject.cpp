#include "PiezoObject.h"
#include "Controler.h"

PiezoObject::PiezoObject(const int &channel, const int &pin) 
    : ArduinoOutputObject(pin), m_playing(false)
{
    m_channel = channel;
    ledcSetup(m_channel, 5000, 8);
    ledcAttachPin(this->pin(), m_channel);
    setType(ArduinoObject::Piezo);
}

void PiezoObject::play()
{
    std::list<PiezoMelody> melodys;
    melodys.push_back({100, 300});
    melodys.push_back({200, 300});
    melodys.push_back({300, 300});
    melodys.push_back({400, 300});
    melodys.push_back({500, 300});
    melodys.push_back({600, 300});
    melodys.push_back({700, 300});
    melodys.push_back({800, 300});
    melodys.push_back({900, 300});
    melodys.push_back({1000, 300});
    melodys.push_back({900, 300});
    melodys.push_back({800, 300});
    melodys.push_back({700, 300});
    melodys.push_back({600, 300});
    melodys.push_back({500, 300});
    melodys.push_back({300, 300});
    melodys.push_back({200, 300});
    melodys.push_back({200, 300});
    melodys.push_back({100, 300});
    play(melodys);
}

void PiezoObject::play(const std::list<PiezoMelody> &melodys)
{
    auto timer = millis();

    m_playing = true;

    for(auto melody : melodys)
    {
        ledcWriteTone(m_channel, melody.frequency);
        timer += melody.duration;
        while(timer > millis())
        {
            if(m_playing)
                Controler::instance()->update();
            else
            {
                ledcWriteTone(m_channel, 0);
                return;
            }
        }
    }
    ledcWriteTone(m_channel, 0);

    m_playing = false;
}

void PiezoObject::stop()
{
    m_playing = false;
}