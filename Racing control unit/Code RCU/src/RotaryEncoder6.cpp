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

    //return -1 decrease 1
    //return 1 increase 1
    //return 0 nothing

    if (readOne == true && readTwo == false && readThree == false && readFour == false && readFive == false && readSix == false && lastPinSelected != 0)
    {
        lastPinSelected = 0;
        return -1;
    }
    else if (readOne == false && readTwo == true && readThree == false && readFour == false && readFive == false && readSix == false && lastPinSelected != 1)
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
    else if (readOne == false && readTwo == false && readThree == true && readFour == false && readFive == false && readSix == false && lastPinSelected != 2)
    {
        if (lastPinSelected == 1)
        {
            lastPinSelected = 2;
            return 1;
        }
        else if (lastPinSelected == 3)
        {
            lastPinSelected = 2;
            return -1;
        }
    }
    else if (readOne == false && readTwo == false && readThree == false && readFour == true && readFive == false && readSix == false && lastPinSelected != 3)
    {
        if (lastPinSelected == 2)
        {
            lastPinSelected = 3;
            return 1;
        }
        else if (lastPinSelected == 4)
        {
            lastPinSelected = 3;
            return -1;
        }
    }
    else if (readOne == false && readTwo == false && readThree == false && readFour == false && readFive == true && readSix == false && lastPinSelected != 4)
    {
        if (lastPinSelected == 3)
        {
            lastPinSelected = 4;
            return 1;
        }
        else if (lastPinSelected == 5)
        {
            lastPinSelected = 4;
            return -1;
        }
    }
    else if (readOne == false && readTwo == false && readThree == false && readFour == false && readFive == false && readSix == true && lastPinSelected != 5)
    {
        lastPinSelected = 5;
        return 1;
    }
    else
    {
        return 0;
    }
}