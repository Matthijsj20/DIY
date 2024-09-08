#include "..\include\KeySwitch.h"

KeySwitch::KeySwitch(int pin) : pin(pin)
{
    pinMode(pin, INPUT);
    CurrentState = digitalRead(pin);
}

KeySwitch::~KeySwitch()
{
}

int KeySwitch::ReadKeySwitch()
{
    value = digitalRead(pin);

    if (value != CurrentState)
    {
        CurrentState = value;
        delay(2);
        return 1;
    }
    else
    {
        return 0;
    }
}