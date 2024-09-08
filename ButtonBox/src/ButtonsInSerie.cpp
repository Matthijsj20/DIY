#include "..\include\ButtonsInSerie.h"

ButtonsInSerie::ButtonsInSerie(int pin) : pin(pin)
{
    pinMode(pin, INPUT);
}

ButtonsInSerie::~ButtonsInSerie()
{
}

int ButtonsInSerie::ReadButtons()
{
    // returns
    //  0 = no button
    //  1 = button 1
    //  2 = button 2
    //  3 = button 3
    //  4 = button 4

    value = analogRead(pin);
    delay(2);
    if (value >= BUTTON1MIN && value <= BUTTON1MAX)
    {
        if (buttonReset)
        {
            buttonReset = false;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (value >= BUTTON2MIN && value <= BUTTON2MAX)
    {
        if (buttonReset)
        {
            buttonReset = false;
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else if (value >= BUTTON3MIN && value <= BUTTON3MAX)
    {
        if (buttonReset)
        {
            buttonReset = false;
            return 3;
        }
        else
        {
            return 0;
        }
    }
    else if (value >= BUTTON4MIN && value <= BUTTON4MAX)
    {

        if (buttonReset)
        {
            buttonReset = false;
            return 4;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        buttonReset = true;
        return 0;
    }
}