#ifndef ROTARYENCODER6_H
#define ROTARYENCODER6_H
#include <Arduino.h>

class RotaryEncoder6
{
private:
    int pinOne;
    int pinTwo;
    int pinThree;
    int pinFour;
    int pinFive;
    int pinSix;
    int lastPinSelected = 0;

public:
    RotaryEncoder6(int firstPin, int secondPin, int thirdPin, int fouredPin, int fifthPin, int sixedPin);
    int ReadEncoder();
};

#endif