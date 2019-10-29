#include "ServoESP.h"

int ServoESP::channel_next_free = 0;

ServoESP::ServoESP() {
    _resetFields();
};

ServoESP::~ServoESP() {
    detach();
}

bool ServoESP::attach(int pin, int channel, 
                   int minAngle, int maxAngle, 
                   int minPulseWidth, int maxPulseWidth) 
{
    if(channel == CHANNEL_NOT_ATTACHED) {
        if(channel_next_free == CHANNEL_MAX_NUM) {
            return false;
        }
        _channel = channel_next_free;
        channel_next_free++;
    } else {
        _channel = channel;
    }

    _pin = pin;
    _minAngle = minAngle;
    _maxAngle = maxAngle;
    _minPulseWidth = minPulseWidth;
    _maxPulseWidth = maxPulseWidth;

    ledcSetup(_channel, 50, 16); // channel X, 50 Hz, 16-bit depth
    ledcAttachPin(_pin, _channel);
    return true;
}


bool ServoESP::detach() {
    if (!this->attached()) {
        return false;
    }

    if(_channel == (channel_next_free - 1))
        channel_next_free--;

    ledcDetachPin(_pin);
    return true;
}

void ServoESP::write(int degrees) {
    degrees = constrain(degrees, _minAngle, _maxAngle);
    writeMicroseconds(_angleToUs(degrees));
}

void ServoESP::writeMicroseconds(int pulseUs) {
    if (!attached()) {
        return;
    }
    pulseUs = constrain(pulseUs, _minPulseWidth, _maxPulseWidth);
    _pulseWidthDuty = _usToDuty(pulseUs);
    ledcWrite(_channel, _pulseWidthDuty);
}

int ServoESP::read() {
    return _usToAngle(readMicroseconds());
}

int ServoESP::readMicroseconds() {
    if (!this->attached()) {
        return 0;
    }
    int duty = ledcRead(_channel);
    return _dutyToUs(duty);
}

bool ServoESP::attached() const { return _pin != PIN_NOT_ATTACHED; }

int ServoESP::attachedPin() const { return _pin; }

void ServoESP::_resetFields(void) {
    _pin = PIN_NOT_ATTACHED;
    _pulseWidthDuty = 0;
    _channel = CHANNEL_NOT_ATTACHED;
    _minAngle = MIN_ANGLE;
    _maxAngle = MAX_ANGLE;
    _minPulseWidth = MIN_PULSE_WIDTH;
    _maxPulseWidth = MAX_PULSE_WIDTH;
}