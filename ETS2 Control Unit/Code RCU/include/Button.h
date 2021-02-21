#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define DEBOUNCETIME 30

class Button
{
private:
    bool ButtonReleased = true;
    int buttonPin;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = DEBOUNCETIME;
    int lastButtonState = LOW;

public:
    Button(int buttonPin);
    int ReadButton();
};

#endif