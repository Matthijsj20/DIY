#include <Arduino.h>
#include <cstring>

#include "ConnectionStatusLeds.hpp"
#include "WifiManager.hpp"
#include "MqttManager.hpp"
#include "pins.hpp"

namespace {
constexpr char kTopic[] = "/doorbell/pressed";
constexpr unsigned long kDebounceDelayMs = 50;

WifiManager wifiManager;
MqttManager mqttManager;
ConnectionStatusLeds statusLeds(LED_RED_PIN, LED_GREEN_PIN);

volatile bool interruptTriggered = false;
unsigned long lastDebounceTimeMs = 0;

void IRAM_ATTR doorbellPressedISR() {
  interruptTriggered = true;
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
  statusLeds.begin();

  pinMode(DOORBELL_PIN, INPUT_PULLUP);
  attachInterrupt(DOORBELL_PIN, doorbellPressedISR, FALLING);

  wifiManager.connect();
}

void loop() {
  wifiManager.connect();
  if (wifiManager.isConnected()) {
    mqttManager.connect();
  }

  statusLeds.update(wifiManager.isConnected(), mqttManager.isConnected());

  mqttManager.loop();

  if (interruptTriggered) {
    handleDoorbellInterrupt();
  }
}
