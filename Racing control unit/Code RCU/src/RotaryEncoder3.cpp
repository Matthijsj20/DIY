#include "RotaryEncoder3.h"

RotaryEncoder3::RotaryEncoder3(int firstPin, int secondPin, int thirdPin) : pinOne(firstPin), pinTwo(secondPin), pinThree(thirdPin)
{
    pinMode(pinOne, INPUT);
    pinMode(pinTwo, INPUT);
    pinMode(pinThree, INPUT);
    int readOne = digitalRead(pinOne);
    int readTwo = digitalRead(pinTwo);
    int readThree = digitalRead(pinThree);
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
}

int RotaryEncoder3::ReadEncoder()
{
    //return -1 decrease 1
    //return 1 increase 1
    //return 0 nothing
    int readOne = digitalRead(pinOne);
    int readTwo = digitalRead(pinTwo);
    int readThree = digitalRead(pinThree);
    if (readOne == HIGH && readTwo == LOW && readThree == LOW && lastPinSelected != 0)
    {
        lastPinSelected = 0;
        return -1;
    }
    else if (readOne == LOW && readTwo == HIGH && readThree == LOW && lastPinSelected != 1)
    {
        if (lastPinSelected == 0)
        {
            lastPinSelected = 1;
            return 1;
        }
        else if (lastPinSelected == 2)
        {
            lastPinSelected = 1;
            return -1;
        }
    }
    else if (readOne == LOW && readTwo == LOW && readThree == HIGH && lastPinSelected != 2)
    {
        lastPinSelected = 2;
        return 1;
    }
    else
    {
        //error
        return 0;
    }
}