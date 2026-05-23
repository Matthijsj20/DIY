#include <Arduino.h>
#include <cstring>

#include "WifiManager.hpp"
#include "MqttManager.hpp"
#include "pins.hpp"

namespace {
WifiManager wifiManager;
MqttManager mqttManager;

bool mqttSubscribed = false;
bool internalLedOn = false;
unsigned long internalLedOffAt = 0;
unsigned long lastBlinkMs = 0;
bool blinkOn = false;

constexpr unsigned long kInternalLedOnDurationMs = 5000;
constexpr unsigned long kLedBlinkIntervalMs = 500;

void setRedLed(bool on) {
  digitalWrite(LED_RED_PIN, on ? HIGH : LOW);
}

void setGreenLed(bool on) {
  digitalWrite(LED_GREEN_PIN, on ? HIGH : LOW);
}

void updateStatusLeds() {
  const unsigned long nowMs = millis();
  if (nowMs - lastBlinkMs >= kLedBlinkIntervalMs) {
    lastBlinkMs = nowMs;
    blinkOn = !blinkOn;
  }

  const bool wifiConnected = wifiManager.isConnected();
  const bool mqttConnected = mqttManager.isConnected();

  if (wifiConnected && mqttConnected) {
    setRedLed(false);
    setGreenLed(true);
    return;
  }

  setGreenLed(blinkOn);

  if (!wifiConnected) {
    setRedLed(true);
  } else {
    setRedLed(blinkOn);
  }
}

void updateInternalLed() {
  if (internalLedOn && millis() >= internalLedOffAt) {
    digitalWrite(INTERNAL_LED_PIN, LOW);
    internalLedOn = false;
  }
}

void onDoorbellPressed() {
  if (internalLedOn) {
    return;
  }
  digitalWrite(INTERNAL_LED_PIN, HIGH);
  internalLedOn = true;
  internalLedOffAt = millis() + kInternalLedOnDurationMs;
}

} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  setRedLed(true);
  setGreenLed(false);
  digitalWrite(INTERNAL_LED_PIN, LOW);

  wifiManager.connect();
}

void loop() {
  updateStatusLeds();

  wifiManager.connect();
  if (wifiManager.isConnected()) {
    mqttManager.connect();
    if (mqttManager.isConnected()) {
      if (!mqttSubscribed) {
        mqttManager.subscribe("/doorbell/pressed");
        mqttSubscribed = true;
      }
    } else {
      mqttSubscribed = false;
    }
  } else {
    mqttSubscribed = false;
  }

  mqttManager.loop();
  updateInternalLed();

  MqttIncomingMessage message;
  if (mqttManager.getMessage(message)) {
    if (message.topic == "/doorbell/pressed") {
      onDoorbellPressed();
    }
  }
}
