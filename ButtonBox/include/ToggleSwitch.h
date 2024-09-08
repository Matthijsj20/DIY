#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <Arduino.h>

class ToggleSwitch
{
private:
    int pin;

    int currentState;
    int value = 0;

public:
    ToggleSwitch(int pin);
    ~ToggleSwitch();
    int ReadToggleSwitch();
};

#endif