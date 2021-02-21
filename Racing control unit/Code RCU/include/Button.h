#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define debounceTime 30

class Button
{
private:
    bool ButtonReleased = true;
    int buttonPin;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = debounceTime;
    int lastButtonState = LOW;

public:
    Button(int buttonPin);
    int ReadButton();
};

#endif