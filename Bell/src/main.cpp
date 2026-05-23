#include <Arduino.h>
#include <cstring>

#include "WifiManager.hpp"
#include "MqttManager.hpp"
#include "pins.hpp"

namespace {
constexpr char kTopic[] = "/doorbell/pressed";
constexpr unsigned long kDebounceDelayMs = 50;
constexpr unsigned long kLedBlinkIntervalMs = 500;

WifiManager wifiManager;
MqttManager mqttManager;

volatile bool interruptTriggered = false;
unsigned long lastDebounceTimeMs = 0;
unsigned long lastBlinkMs = 0;
bool blinkOn = false;

void IRAM_ATTR doorbellPressedISR() {
  interruptTriggered = true;
}

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

void publishDoorbellState() {
  if (!mqttManager.publish(kTopic, "true")) {
    Serial.println("MQTT publish failed");
    return;
  }
  Serial.println("MQTT publish success: Doorbell pressed");
}

void handleDoorbellInterrupt() {
  const unsigned long nowMs = millis();
  if (nowMs - lastDebounceTimeMs < kDebounceDelayMs) {
    return;
  }

  interruptTriggered = false;
  if (digitalRead(DOORBELL_PIN) != LOW) {
    return;
  }

  lastDebounceTimeMs = nowMs;
  publishDoorbellState();
}
} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  setRedLed(true);
  setGreenLed(false);

  pinMode(DOORBELL_PIN, INPUT_PULLUP);
  attachInterrupt(DOORBELL_PIN, doorbellPressedISR, FALLING);

  wifiManager.connect();
}

void loop() {
  updateStatusLeds();

  wifiManager.connect();
  if (wifiManager.isConnected()) {
    mqttManager.connect();
  }

  mqttManager.loop();

  if (interruptTriggered) {
    handleDoorbellInterrupt();
  }
}
