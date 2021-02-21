#ifndef VIBRATIONSENSOR_H
#define VIBRATIONSENSOR_H

#include <Arduino.h>

class vibrationSensor
{
private:
    int sensorPin;

public:
    vibrationSensor(int sensorPin);
    ~vibrationSensor();
    int readVibrationSensor();
};

#endif