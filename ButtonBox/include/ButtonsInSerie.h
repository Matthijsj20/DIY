#ifndef BUTTONSINSERIE_H
#define BUTTONSINSERIE_H

#include <Arduino.h>

#define BUTTON1MIN 150
#define BUTTON1MAX 250

#define BUTTON2MIN 350
#define BUTTON2MAX 450

#define BUTTON3MIN 800
#define BUTTON3MAX 900

#define BUTTON4MIN 3400
#define BUTTON4MAX 4095

class ButtonsInSerie
{
private:
    int pin;
    bool buttonReset = true;
    int value = 0;

public:
    ButtonsInSerie(int pin);
    ~ButtonsInSerie();
    int ReadButtons();
};

#endif