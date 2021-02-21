#ifndef POTMETER_H
#define POTMETER_H
#include <Arduino.h>

class PotMeter
{
private:
    int sensorPin;

public:
    PotMeter(int sensorPin);
    ~PotMeter();
    int readPotMeter();
};

#endif //POTMETER_H