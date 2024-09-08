#ifndef KEYSWITCH_H
#define KEYSWITCH_H

#include <Arduino.h>

class KeySwitch
{
private:
    int pin;
    int CurrentState;
    int value = 0;

public:
    KeySwitch(int pin);
    ~KeySwitch();
    int ReadKeySwitch();
};

#endif