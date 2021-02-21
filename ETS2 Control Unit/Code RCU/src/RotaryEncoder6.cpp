#include "RotaryEncoder6.h"

RotaryEncoder6::RotaryEncoder6(int firstPin, int secondPin, int thirdPin, int fouredPin, int fifthPin, int sixedPin)
    : pinOne(firstPin), pinTwo(secondPin), pinThree(thirdPin), pinFour(fouredPin), pinFive(fifthPin), pinSix(sixedPin)
{
    pinMode(pinOne, INPUT);
    pinMode(pinTwo, INPUT);
    pinMode(pinThree, INPUT);
    pinMode(pinFour, INPUT);
    pinMode(pinFive, INPUT);
    pinMode(pinSix, INPUT);
    int readOne = digitalRead(pinOne);
    int readTwo = digitalRead(pinTwo);
    int readThree = digitalRead(pinThree);
    int readFour = digitalRead(pinFour);
    int readFive = digitalRead(pinFive);
    int readSix = digitalRead(pinSix);
    if (readOne == HIGH)
    {
        lastPinSelected = 0;
    }
    else if (readTwo == HIGH)
    {
        lastPinSelected = 1;
    }
    else if (readThree == HIGH)
    {
        lastPinSelected = 2;
    }
    else if (readFour == HIGH)
    {
        lastPinSelected = 3;
    }
    else if (readFive == HIGH)
    {
        lastPinSelected = 4;
    }
    else if (readSix == HIGH)
    {
        lastPinSelected = 5;
    }
}

int RotaryEncoder6::ReadEncoder()
{
    int readOne = digitalRead(pinOne);
    int readTwo = digitalRead(pinTwo);
    int readThree = digitalRead(pinThree);
    int readFour = digitalRead(pinFour);
    int readFive = digitalRead(pinFive);
    int readSix = digitalRead(pinSix);

    //return 0 off
    //return 1 stadlicht
    //return 2 dimlicht
    //return 3 grootlicht

    if (readOne == HIGH && readTwo == LOW && readThree == LOW && readFour == LOW && lastPinSelected != 0)
    {
        lastPinSelected = 0;
        return 0;
    }
    else if (readOne == LOW && readTwo == HIGH && readThree == LOW && readFour == LOW && lastPinSelected != 1)
    {
        lastPinSelected = 1;
        return 1;
    }
    else if (readOne == LOW && readTwo == LOW && readThree == HIGH && readFour == LOW && lastPinSelected != 2)
    {
        lastPinSelected = 2;
        return 2;
    }
    else if (readOne == LOW && readTwo == LOW && readThree == LOW && readFour == HIGH && lastPinSelected != 3)
    {
        lastPinSelected = 3;
        return 3;
    }
    else
    {
        return -1;
    }
}