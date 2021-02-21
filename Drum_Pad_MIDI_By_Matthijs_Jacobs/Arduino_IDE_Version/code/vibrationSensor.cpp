#include "vibrationSensor.h"

vibrationSensor::vibrationSensor(int sensorPin) : sensorPin(sensorPin)
{
    pinMode(sensorPin, INPUT);
}

vibrationSensor::~vibrationSensor()
{
}

int vibrationSensor::readVibrationSensor()
{
    return analogRead(sensorPin);
}