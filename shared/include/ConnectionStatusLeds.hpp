#pragma once

#include <Arduino.h>

class ConnectionStatusLeds {
public:
  ConnectionStatusLeds(uint8_t redPin, uint8_t greenPin);

  void begin();
  void update(bool wifiConnected, bool mqttConnected);

private:
  void setRed(bool on);
  void setGreen(bool on);

  uint8_t redPin_;
  uint8_t greenPin_;
  unsigned long lastBlinkMs_ = 0;
  bool blinkOn_ = false;

  static constexpr unsigned long kBlinkIntervalMs = 500;
};
