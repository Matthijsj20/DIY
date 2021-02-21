#ifndef BRAKEBIAS_H
#define BRAKEBIAS_H
#include <Arduino.h>

#define minADC 0
#define maxADC 4095
#define minGameValue 50
#define maxGameValue 70
class Brakebias
{
private:
    int potPin;

public:
    Brakebias(int potPin);
    int ReadPotMeter();
};
#endif