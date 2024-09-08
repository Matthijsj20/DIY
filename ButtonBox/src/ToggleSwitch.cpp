#include "..\include\ToggleSwitch.h"

ToggleSwitch::ToggleSwitch(int pin) : pin(pin)
{
    pinMode(pin, INPUT);
    currentState = digitalRead(pin);
}

ToggleSwitch::~ToggleSwitch()
{
}

int ToggleSwitch::ReadToggleSwitch()
{
    value = digitalRead(pin);
    if (value != currentState)
    {
        currentState = value;
        delay(2);
        return 1;
    }
    else
    {
        return 0;
    }
}
