#include "Brakebias.h"

Brakebias::Brakebias(int potPin) : potPin(potPin)
{
    //set pinmode
    pinMode(potPin, INPUT);
}

int Brakebias::ReadPotMeter()
{
    int potReading = map(analogRead(potPin), minADC, maxADC, minGameValue, maxGameValue);
    return potReading;
}