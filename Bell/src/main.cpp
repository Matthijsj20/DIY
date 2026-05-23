#include <Arduino.h>
#include <cstring>

#include "WiFi/WifiManager.hpp"
#include "Mqtt/MqttManager.hpp"
#include "pins.hpp"

namespace {
constexpr char kTopic[] = "/doorbell/pressed";
constexpr unsigned long kDebounceDelayMs = 50;

WifiManager wifiManager;
MqttManager mqttManager;

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

  wifiManager.connect(); // Blocking call until connected
  mqttManager.connect(); // Blocking call until connected

  pinMode(DOORBELL_PIN, INPUT_PULLUP);
  attachInterrupt(DOORBELL_PIN, doorbellPressedISR, FALLING);
}

void loop() {
  if (!wifiManager.isConnected()) {
    Serial.println("WiFi lost, reconnecting...");
    wifiManager.connect(); // Blocking call until connected
  }

  if (!mqttManager.isConnected()) {
    Serial.println("MQTT broker connection lost, reconnecting...");
    mqttManager.connect(); // Blocking call until connected
  }

  mqttManager.loop();

  if (interruptTriggered) {
    handleDoorbellInterrupt();
  }
}
