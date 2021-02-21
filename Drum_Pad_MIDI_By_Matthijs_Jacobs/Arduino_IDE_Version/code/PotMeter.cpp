#include "PotMeter.h"

PotMeter::PotMeter(int sensorPin) : sensorPin(sensorPin)
{
    pinMode(sensorPin, INPUT);
}

PotMeter::~PotMeter()
{
}

int PotMeter::readPotMeter()
{
    return analogRead(sensorPin);
}