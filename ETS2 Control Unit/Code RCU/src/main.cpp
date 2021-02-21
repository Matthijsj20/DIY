#include <BleKeyboard.h>
#include <LiquidCrystal.h>
#include "Button.h"
#include "Keys.h"
#include "mapping.h"
#include "states.h"
#include "RotaryEncoder3.h"
#include "RotaryEncoder6.h"

#define BUTTONREADGAMEDELAY 85 //with some games 1 clock cycle isn't enough te detect a button press so button needs to be pressed for a certain amount of time in milli seconds

BleKeyboard bleKeyboard;
Button difLock(DIFLOCKPIN);
Button alarmLight(ALARMPIN);
RotaryEncoder3 keysEncoder(FIRSTKEYPIN, SECONDKEYPIN, THIRDKEYPIN);
RotaryEncoder6 lightsEncoder(FIRSTLIGHTPIN, SECONDLIGHTPIN, THIRDLIGHTPIN, FOURTHLIGHTPIN, FIRSTLIGHTPIN, SIXEDLIGHTPIN);
states currentState = WaitForConnection;
lightStates currentLightState = OFF;

unsigned long delayTime = 100;
unsigned long KeyslastTime = 0;
unsigned long lightLastTime = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("BOOT");
  bleKeyboard.begin();
}

void loop()
{
  //Serial.println(currentState);
  switch (currentState)
  {
  case WaitForConnection:
    if (bleKeyboard.isConnected())
    {
      currentState = Connected;
    }
    break;

  case Connected:
    if (bleKeyboard.isConnected())
    {
      //KEYSENCODER
      if (KeyslastTime + delayTime < millis())
      {
        KeyslastTime = millis();

        int keysEncoderResult = keysEncoder.ReadEncoder();
        if (digitalRead(DIFLOCKPIN) != HIGH)
        {
          switch (keysEncoderResult)
          {
          case -1:
            //turn engine off
            bleKeyboard.press(ELECTRICS);
            delay(BUTTONREADGAMEDELAY);
            bleKeyboard.release(ELECTRICS);
            break;

          case 0:
            //nothing
            break;

          case 1:
            //turn electrics on
            bleKeyboard.press(ELECTRICS);
            delay(BUTTONREADGAMEDELAY);
            bleKeyboard.release(ELECTRICS);
            break;

          case 2:
            //turn engine on
            bleKeyboard.press(ENGINE);
            delay(BUTTONREADGAMEDELAY);
            bleKeyboard.release(ENGINE);
            break;

          case 3:
            //turn electrics and engine on
            bleKeyboard.press(ELECTRICS);
            delay(BUTTONREADGAMEDELAY);
            bleKeyboard.release(ELECTRICS);
            bleKeyboard.press(ENGINE);
            delay(BUTTONREADGAMEDELAY);
            bleKeyboard.release(ENGINE);
            break;

          default:
            break;
          }
        }
      }
      //END KEYSENCODER

      //LIGHT
      if (lightLastTime + delayTime < millis())
      {
        lightLastTime = millis();

        int lightResult = lightsEncoder.ReadEncoder();
        if (lightResult != -1)
        {
          Serial.println(lightResult);
          Serial.print("STATE: ");
          Serial.println(currentLightState);
        }
        if (digitalRead(DIFLOCKPIN) != HIGH)
        {
          switch (lightResult)
          {
          case 0:
            if (currentLightState == CITYLIGHT)
            {
              //2 cycles
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
              delay(50);
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            else if (currentLightState == DIMLIGHT)
            {
              //1 cycle
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            else if (currentLightState == HUGELIGHT)
            {
              //trigger switch
              bleKeyboard.press(HUGELICHTSWITCH);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(HUGELICHTSWITCH);
              delay(50);
              //1 cycle
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            currentLightState = OFF;
            break;

          case 1:
            if (currentLightState == OFF)
            {
              //1 cycle
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            else if (currentLightState == DIMLIGHT)
            {
              //2 cycle
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
              delay(50);
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            else if (currentLightState == HUGELIGHT)
            {
              //trigger switch
              bleKeyboard.press(HUGELICHTSWITCH);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(HUGELICHTSWITCH);
              delay(50);
              //2 cycle
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
              delay(50);
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            currentLightState = CITYLIGHT;
            break;

          case 2:
            if (currentLightState == OFF)
            {
              //cycle 2
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
              delay(50);
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            else if (currentLightState == CITYLIGHT)
            {
              //cycle 1
              bleKeyboard.press(LIGHTCYCLE);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(LIGHTCYCLE);
            }
            else if (currentLightState == HUGELIGHT)
            {
              //trigger switch
              bleKeyboard.press(HUGELICHTSWITCH);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(HUGELICHTSWITCH);
            }
            currentLightState = DIMLIGHT;
            break;

          case 3:
            if (currentLightState == OFF)
            {
              //trigger switch
              bleKeyboard.press(HUGELICHTSWITCH);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(HUGELICHTSWITCH);
            }
            else if (currentLightState == CITYLIGHT)
            {
              //trigger switch
              bleKeyboard.press(HUGELICHTSWITCH);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(HUGELICHTSWITCH);
            }
            else if (currentLightState == DIMLIGHT)
            {
              //trigger switch
              bleKeyboard.press(HUGELICHTSWITCH);
              delay(BUTTONREADGAMEDELAY);
              bleKeyboard.release(HUGELICHTSWITCH);
            }
            currentLightState = HUGELIGHT;
            break;

          default:
            break;
          }
        }
        else
        {
          if (digitalRead(FIRSTLIGHTPIN == HIGH))
          {
            currentLightState = OFF;
          }
          else if (digitalRead(SECONDLIGHTPIN == HIGH))
          {
            currentLightState = CITYLIGHT;
          }
          else if (digitalRead(THIRDKEYPIN == HIGH))
          {
            currentLightState = DIMLIGHT;
          }
          else if (digitalRead(FOURTHLIGHTPIN == HIGH))
          {
            currentLightState = HUGELIGHT;
          }
        }
      }
      //END LIGHT

      //DIFLOCK
      if (difLock.ReadButton() == HIGH)
      {
        delay(250);
        if (digitalRead(DIFLOCKPIN) != HIGH) //prevent button press when long press to reset engine or light mode
        {
          bleKeyboard.press(DIFLOCK);
          delay(BUTTONREADGAMEDELAY);
          bleKeyboard.release(DIFLOCK);
        }
      }
      //END DIFLOCK

      //ALARMLIGHT
      if (alarmLight.ReadButton() == HIGH)
      {
        bleKeyboard.press(ALARMLIGHTS);
        delay(BUTTONREADGAMEDELAY);
        bleKeyboard.release(ALARMLIGHTS);
      }
      //END ALARMLIGHT
    }
    else
    {
      //disconnected
      delay(1000);
      currentState = WaitForConnection;
    }
    break;

  default:
    break;
  }
}
