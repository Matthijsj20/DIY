#include <Arduino.h>
#include <cstring>

#include "WiFi/WifiManager.hpp"
#include "Mqtt/MqttManager.hpp"
#include "pins.hpp"

namespace {
WifiManager wifiManager;
MqttManager mqttManager;

} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(INTERNAL_LED_PIN, OUTPUT);

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
}
