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
    //return -1 turn engine off
    //return 0 nothing
    //return 1 turn electrics on
    //return 2 turn engine on
    //return 3 turn engine and electrics on

    int readOne = digitalRead(pinOne);
    int readTwo = digitalRead(pinTwo);
    int readThree = digitalRead(pinThree);
    if (readOne == HIGH && readTwo == LOW && readThree == LOW && lastPinSelected == 1)
    {
        lastPinSelected = 0;
        //turn engine off
        return -1;
    }
    else if (readOne == HIGH && readTwo == LOW && readThree == LOW && lastPinSelected == 2)
    {
        lastPinSelected = 0;
        //turn engine off
        return -1;
    }

    else if (readOne == LOW && readTwo == HIGH && readThree == LOW && lastPinSelected == 0)
    {
        lastPinSelected = 1;
        //turn electrics on
        return 1;
    }
    else if (readOne == LOW && readTwo == HIGH && readThree == LOW && lastPinSelected == 2)
    {
        lastPinSelected = 1;
        //turn engine on
        return 2;
    }

    else if (readOne == LOW && readTwo == LOW && readThree == HIGH && lastPinSelected == 0)
    {
        lastPinSelected = 2;
        //turn electrics + engine on
        return 3;
    }
    else if (readOne == LOW && readTwo == LOW && readThree == HIGH && lastPinSelected == 1)
    {
        lastPinSelected = 2;
        //turn engine on
        return 2;
    }
    else
    {
        return 0;
    }
}