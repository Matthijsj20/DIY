#include "MqttManager.hpp"

#include <cstring>

MqttManager::MqttManager() : mqtt(wifiClient) {
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
}

MqttManager::~MqttManager() {
}

bool MqttManager::isConnected() {
  return mqtt.connected();
}

void MqttManager::connect() {
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT broker");
  while (!mqtt.connected()) {
    const bool connected = strlen(MQTT_USER) > 0
                               ? mqtt.connect("doorbell-detector", MQTT_USER, MQTT_PASS)
                               : mqtt.connect("doorbell-detector");
    if (!connected) {
      delay(2000);
      Serial.print('.');
      yield();
    }
  }
  Serial.println();
  Serial.println("MQTT connected");
}

void MqttManager::loop() {
  mqtt.loop();
}

bool MqttManager::publish(const char *topic, const char *message) {
  return mqtt.publish(topic, message);
}

void MqttManager::onMqttMessage(char *topic, byte *payload, unsigned int length) {
  char message[256];
  const size_t copyLen = length < sizeof(message) - 1 ? length : sizeof(message) - 1;
  memcpy(message, payload, copyLen);
  message[copyLen] = '\0';
  Serial.printf("MQTT RX %s -> %s\n", topic, message);
}