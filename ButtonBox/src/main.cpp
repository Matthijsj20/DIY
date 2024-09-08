// System libs
#include <Arduino.h>
#include <BleKeyboard.h>

// Own libs
#include "..\include\States.h"
#include "..\include\PinOut.h"
#include "..\include\Keys.h"
#include "..\include\ToggleSwitch.h"
#include "..\include\ButtonsInSerie.h"
#include "..\include\KeySwitch.h"
#include "..\include\RotaryEncoder.h"

// Defines
#define HoldPressDelay 85

// Global vars
BleKeyboard Keyboard;
States CurrentState = WAITINGFORCONNECTION;

ToggleSwitch TS1(TOGGLESWITCH1PIN);
ToggleSwitch TS2(TOGGLESWITCH2PIN);
ToggleSwitch TS3(TOGGLESWITCH3PIN);
ToggleSwitch TS4(TOGGLESWITCH4PIN);

ButtonsInSerie BRS(BUTTONREDSERIEPIN);
ButtonsInSerie BBS(BUTTONBLACKSERIEPIN);

KeySwitch KSS(STARTKEYPIN);
ToggleSwitch TSS(STARTTOGGLEPIN);

RotaryEncoder R1(ROTARYENCODER1APIN, ROTARYENCODER1BPIN);
RotaryEncoder R2(ROTARYENCODER2APIN, ROTARYENCODER2BPIN);
RotaryEncoder R3(ROTARYENCODER3APIN, ROTARYENCODER3BPIN);
RotaryEncoder R4(ROTARYENCODER4APIN, ROTARYENCODER4BPIN);

int ButtonRedValue = 0;
int ButtonBlackValue = 0;

int R1Value = 0;
int R2Value = 0;
int R3Value = 0;
int R4Value = 0;

void PressKey(char key, unsigned long holdPressDelay)
{
  Keyboard.press(key);
  delay(holdPressDelay);
  Keyboard.release(key);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("BOOT");
  Keyboard.deviceName = "ButtonBox_Matthijs";
  Keyboard.batteryLevel = 100;
  Keyboard.deviceManufacturer = "Jacobs Inc.";
  Keyboard.begin();
}

void loop()
{
  switch (CurrentState)
  {
  case WAITINGFORCONNECTION:
    if (Keyboard.isConnected())
    {
      CurrentState = CONNECTED;
    }
    break;

  case CONNECTED:
    // Connection Check
    if (!Keyboard.isConnected())
    {
      CurrentState = WAITINGFORCONNECTION;
    }

    // Read ToggleSwitches
    if (TS1.ReadToggleSwitch())
    {
      PressKey(TOGGLESWITCH1KEY, HoldPressDelay);
    }

    if (TS2.ReadToggleSwitch())
    {
      PressKey(TOGGLESWITCH2KEY, HoldPressDelay);
    }

    if (TS3.ReadToggleSwitch())
    {
      PressKey(TOGGLESWITCH3KEY, HoldPressDelay);
    }

    if (TS4.ReadToggleSwitch())
    {
      PressKey(TOGGLESWITCH4KEY, HoldPressDelay);
    }

    // Read Buttons
    ButtonRedValue = BRS.ReadButtons();
    if (ButtonRedValue == 1)
    {
      PressKey(BUTTONRED1KEY, HoldPressDelay);
    }
    else if (ButtonRedValue == 2)
    {
      PressKey(BUTTONRED2KEY, HoldPressDelay);
    }
    else if (ButtonRedValue == 3)
    {
      PressKey(BUTTONRED3KEY, HoldPressDelay);
    }
    else if (ButtonRedValue == 4)
    {
      PressKey(BUTTONRED4KEY, HoldPressDelay);
    }
    ButtonBlackValue = BBS.ReadButtons();
    if (ButtonBlackValue == 1)
    {
      PressKey(BUTTONBLACK1KEY, HoldPressDelay);
    }
    else if (ButtonBlackValue == 2)
    {
      PressKey(BUTTONBLACK2KEY, HoldPressDelay);
    }
    else if (ButtonBlackValue == 3)
    {
      PressKey(BUTTONBLACK3KEY, HoldPressDelay);
    }
    else if (ButtonBlackValue == 4)
    {
      PressKey(BUTTONBLACK4KEY, HoldPressDelay);
    }

    // Read Rotary encoders
    R1Value = R1.ReadRotaryEncoder();
    if (R1Value == 1)
    {
      PressKey(R1CLOCKWISEKEY, HoldPressDelay);
    }
    else if (R1Value == -1)
    {
      PressKey(R1COUNTERCLOCKWISEKEY, HoldPressDelay);
    }
    else if (R1Value == 2)
    {
      PressKey(R1BUTTONPRESSKEY, HoldPressDelay);
    }
    R2Value = R2.ReadRotaryEncoder();
    if (R2Value == 1)
    {
      PressKey(R2CLOCKWISEKEY, HoldPressDelay);
    }
    else if (R2Value == -1)
    {
      PressKey(R2COUNTERCLOCKWISEKEY, HoldPressDelay);
    }
    else if (R2Value == 2)
    {
      PressKey(R2BUTTONPRESSKEY, HoldPressDelay);
    }
    R3Value = R3.ReadRotaryEncoder();
    if (R3Value == 1)
    {
      PressKey(R3CLOCKWISEKEY, HoldPressDelay);
    }
    else if (R3Value == -1)
    {
      PressKey(R3COUNTERCLOCKWISEKEY, HoldPressDelay);
    }
    else if (R3Value == 2)
    {
      PressKey(R3BUTTONPRESSKEY, HoldPressDelay);
    }
    R4Value = R4.ReadRotaryEncoder();
    if (R4Value == 1)
    {
      PressKey(R4CLOCKWISEKEY, HoldPressDelay);
    }
    else if (R4Value == -1)
    {
      PressKey(R4COUNTERCLOCKWISEKEY, HoldPressDelay);
    }
    else if (R4Value == 2)
    {
      PressKey(R4BUTTONPRESSKEY, HoldPressDelay);
    }

    // Read Starters
    if (KSS.ReadKeySwitch())
    {
      PressKey(STARTKEYKEY, HoldPressDelay);
    }

    if (TSS.ReadToggleSwitch())
    {
      PressKey(STARTTOGGLEKEY, HoldPressDelay);
    }

    break;
  }
}