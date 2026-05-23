#include <Arduino.h>
#include <cstring>

#include "WiFi/WifiManager.hpp"
#include "Mqtt/MqttManager.hpp"
#include "pins.hpp"

namespace {
constexpr char kTopic[] = "/doorbell/pressed";
constexpr unsigned long kPublishIntervalMs = 1000;

WifiManager wifiManager;
MqttManager mqttManager;

bool doorbellPressed = false;

void IRAM_ATTR doorbellPressedISR() {
  doorbellPressed = true;
}

void publishDoorbellState() {
  if (!mqttManager.publish(kTopic, "true")) {
    Serial.println("MQTT publish failed");
    return;
  }
  Serial.println("MQTT publish success: Doorbell pressed");
}
} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(DOORBELL_PIN, INPUT_PULLUP);
  attachInterrupt(DOORBELL_PIN, doorbellPressedISR, FALLING);

  wifiManager.connect(); // Blocking call until connected
  mqttManager.connect(); // Blocking call until connected
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

  if (doorbellPressed) {
    doorbellPressed = !doorbellPressed;
    publishDoorbellState();
  }
}
