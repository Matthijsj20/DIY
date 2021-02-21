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
Button nextCameraButton(NEXTCAMERAPIN);
Button pitLimiterButton(PITLIMITERPIN);
RotaryEncoder3 fuelMixEncoder(FIRSTFUELPIN, SECONDFUELPIN, THIRDFUELPIN);
RotaryEncoder6 ERSEncoder(FIRSTERSPIN, SECONDERSPIN, THIRDERSPIN, FOURTHERSPIN, FIFTHERSPIN, SIXEDERSPIN);
LiquidCrystal lcd(LCDRS, LCDEN, LCDD4, LCDD5, LCDD6, LCDD7);
states currentState = WaitForConnection;

unsigned long delayTime = 100;
unsigned long lastTime = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("BOOT");
  pinMode(LCDDISPLATTOGGLEPIN, OUTPUT);
  digitalWrite(LCDDISPLATTOGGLEPIN, HIGH);
  bleKeyboard.begin();
  lcd.begin(16, 2);
  lcd.print("Waitng for");
  lcd.setCursor(0, 1);
  lcd.print("connection...");
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
      lcd.clear();
      lcd.print("Connected!");
    }
    break;

  case Connected:
    if (bleKeyboard.isConnected())
    {

      if (lastTime + delayTime < millis())
      {
        lastTime = millis();
        //FUEL
        int fuelEncoderResult = fuelMixEncoder.ReadEncoder();
        if (fuelEncoderResult == 1)
        {
          //increase fuel
          bleKeyboard.press(FUELMIXUP);
          delay(BUTTONREADGAMEDELAY);
          bleKeyboard.release(FUELMIXUP);
        }
        else if (fuelEncoderResult == -1)
        {
          //decrease fuel
          bleKeyboard.press(FUELMIXDOWN);
          delay(BUTTONREADGAMEDELAY);
          bleKeyboard.release(FUELMIXDOWN);
        }
        // //END FUEL
      }

      // //ERS
      // int ERSEncoderResult = ERSEncoder.ReadEncoder();
      // if (ERSEncoderResult == 1)
      // {
      //   //increase ERS

      //   // bleKeyboard.press(ERSUP);
      //   // delay(BUTTONREADGAMEDELAY);
      //   // bleKeyboard.release(ERSUP);
      // }
      // else if (ERSEncoderResult == -1)
      // {
      //   //decrease ERS
      // }
      // //END ERS

      //NEXTCAMERA
      if (nextCameraButton.ReadButton() == 1)
      {
        bleKeyboard.press(NEXTCAMERAKEY);
        delay(BUTTONREADGAMEDELAY);
        bleKeyboard.release(NEXTCAMERAKEY);
        lcd.clear();
        lcd.print("Last Pressed: ");
        lcd.setCursor(0, 1);
        lcd.print("Next camera");
      }
      //END NEXTCAMERA

      //PITLIMITER
      if (pitLimiterButton.ReadButton() == 1)
      {
        bleKeyboard.press(PITLIMITERKEY);
        delay(BUTTONREADGAMEDELAY);
        bleKeyboard.release(PITLIMITERKEY);
        lcd.clear();
        lcd.print("Last Pressed: ");
        lcd.setCursor(0, 1);
        lcd.print("Pit Limiter");
      }
      //END PITLIMITER
    }
    else
    {
      lcd.clear();
      lcd.print("Connection Lost...");
      delay(1000);
      lcd.clear();
      lcd.print("Waitng for");
      lcd.setCursor(0, 1);
      lcd.print("connection...");
      currentState = WaitForConnection;
    }
    break;

  default:
    break;
  }
}