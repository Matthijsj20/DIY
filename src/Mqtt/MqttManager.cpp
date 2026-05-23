#include "MqttManager.hpp"

MqttManager::MqttManager() {
  mqtt = PubSubClient(wifiClient);
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

  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);

  Serial.print("Connecting to MQTT broker");
  while (!mqtt.connected()) {
    const bool connected = strlen(MQTT_USER) > 0
                               ? mqtt.connect("esp32-doorbell", MQTT_USER, MQTT_PASS)
                               : mqtt.connect("esp32-doorbell");
    if (!connected) {
      delay(2000);
      Serial.print('.');
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
  Serial.printf("MQTT RX %s -> %s\n", topic, payload);
}