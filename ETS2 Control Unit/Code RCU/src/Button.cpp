#include "Button.h"

Button::Button(int buttonPin) : buttonPin(buttonPin)
{
    //set pinmode
    pinMode(buttonPin, INPUT);
}

int Button::ReadButton()
{
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (digitalRead(buttonPin) == HIGH)
        {
            if (ButtonReleased == true)
            {
                ButtonReleased = false;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            ButtonReleased = true;
            return 0;
        }
    }
    else
    {
        lastButtonState = reading;
        return 0;
    }
}