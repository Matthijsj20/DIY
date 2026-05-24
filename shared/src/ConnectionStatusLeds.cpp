#include "ConnectionStatusLeds.hpp"

ConnectionStatusLeds::ConnectionStatusLeds(uint8_t redPin, uint8_t greenPin)
    : redPin_(redPin), greenPin_(greenPin) {}

void ConnectionStatusLeds::begin() {
  pinMode(redPin_, OUTPUT);
  pinMode(greenPin_, OUTPUT);
  setRed(true);
  setGreen(false);
}

void ConnectionStatusLeds::update(bool wifiConnected, bool mqttConnected) {
  const unsigned long nowMs = millis();
  if (nowMs - lastBlinkMs_ >= kBlinkIntervalMs) {
    lastBlinkMs_ = nowMs;
    blinkOn_ = !blinkOn_;
  }

  if (wifiConnected && mqttConnected) {
    setRed(false);
    setGreen(true);
    return;
  }

  setGreen(blinkOn_);

  if (!wifiConnected) {
    setRed(true);
  } else {
    setRed(blinkOn_);
  }
}

void ConnectionStatusLeds::setRed(bool on) {
  digitalWrite(redPin_, on ? HIGH : LOW);
}

void ConnectionStatusLeds::setGreen(bool on) {
  digitalWrite(greenPin_, on ? HIGH : LOW);
}
