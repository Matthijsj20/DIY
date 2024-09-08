#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>

class RotaryEncoder
{
private:
    int pinA;
    int pinB;

    bool aTriggerd = false;
    bool bTriggerd = false;

    unsigned long aStartTime = 0;
    unsigned long bStartTime = 0;
    unsigned long TimeDelay = 100;

public:
    RotaryEncoder(int pinA, int pinB);
    ~RotaryEncoder();
    int ReadRotaryEncoder();
};

#endif