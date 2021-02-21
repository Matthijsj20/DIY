#ifndef ROTARYENCODER3_H
#define ROTARYENCODER3_H
#include <Arduino.h>

class RotaryEncoder3
{
private:
    int pinOne;
    int pinTwo;
    int pinThree;
    int lastPinSelected = 0;

public:
    RotaryEncoder3(int firstPin, int secondPin, int thirdPin);
    int ReadEncoder();
};

#endif