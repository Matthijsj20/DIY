#include "..\include\RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int pinA, int pinB) : pinA(pinA), pinB(pinB)
{
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
}

RotaryEncoder::~RotaryEncoder()
{
}

int RotaryEncoder::ReadRotaryEncoder()
{
    // return values
    // -1 Counter clockwise
    // 0 nothing
    // 1 clockwise
    // 2 ButtonClick

    if (digitalRead(pinA) && !digitalRead(pinB) && !aTriggerd && !bTriggerd)
    {
        aTriggerd = true;
        aStartTime = millis();
    }

    if (digitalRead(pinB) && !digitalRead(pinA) && !bTriggerd && !aTriggerd)
    {
        bTriggerd = true;
        bStartTime = millis();
    }

    while (aTriggerd)
    {
        if (digitalRead(pinB))
        {
            aTriggerd = false;
            delay(14);
            return 1;
        }
        if (millis() > (aStartTime + TimeDelay))
        {
            aTriggerd = false;
            break;
        }
    }

    while (bTriggerd)
    {
        if (digitalRead(pinA))
        {
            bTriggerd = false;
            delay(14);
            return -1;
        }
        if (millis() > (bStartTime + TimeDelay))
        {
            bTriggerd = false;
            break;
        }
    }
    return 0;
}