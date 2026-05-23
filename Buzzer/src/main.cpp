#include <Arduino.h>
#include <cstring>

#include "WifiManager.hpp"
#include "MqttManager.hpp"
#include "pins.hpp"

namespace {
WifiManager wifiManager;
MqttManager mqttManager;

bool ledOn = false;
unsigned long ledOffAt = 0;

constexpr unsigned long kLedOnDurationMs = 5000;

void updateLed() {
  if (ledOn && millis() >= ledOffAt) {
    digitalWrite(INTERNAL_LED_PIN, LOW);
    ledOn = false;
  }
}

void onDoorbellPressed() {
  if (ledOn) {
    return;
  }
  digitalWrite(INTERNAL_LED_PIN, HIGH);
  ledOn = true;
  ledOffAt = millis() + kLedOnDurationMs;
}

} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(INTERNAL_LED_PIN, OUTPUT);

  wifiManager.connect(); // Blocking call until connected
  mqttManager.connect(); // Blocking call until connected
  mqttManager.subscribe("/doorbell/pressed");
}

void loop() {
  if (!wifiManager.isConnected()) {
    Serial.println("WiFi lost, reconnecting...");
    wifiManager.connect(); // Blocking call until connected
  }

  if (!mqttManager.isConnected()) {
    Serial.println("MQTT broker connection lost, reconnecting...");
    mqttManager.connect(); // Blocking call until connected
    mqttManager.subscribe("/doorbell/pressed");
  }

  mqttManager.loop();
  updateLed();

  MqttIncomingMessage message;
  if (mqttManager.getMessage(message)) {
    if (message.topic == "/doorbell/pressed") {
      onDoorbellPressed();
    }
  }
}
