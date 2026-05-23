#include "MqttManager.hpp"

#include <cstring>

MqttManager *MqttManager::instance = nullptr;

MqttManager::MqttManager() : mqtt(wifiClient) {
  instance = this;
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
}

MqttManager::~MqttManager() {}

bool MqttManager::isConnected() { return mqtt.connected(); }

void MqttManager::connect() {
  if (mqtt.connected()) {
    if (!loggedConnected) {
      Serial.println("MQTT connected");
      loggedConnected = true;
      connectingLogged = false;
    }
    return;
  }

  loggedConnected = false;

  const unsigned long now = millis();
  if (now - lastAttemptMs < kRetryIntervalMs) {
    return;
  }
  lastAttemptMs = now;

  if (!connectingLogged) {
    Serial.print("Connecting to MQTT broker");
    connectingLogged = true;
  }

  const bool connected =
      strlen(MQTT_USER) > 0
          ? mqtt.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)
          : mqtt.connect(MQTT_CLIENT_ID);

  if (connected) {
    Serial.println();
    Serial.println("MQTT connected");
    loggedConnected = true;
    connectingLogged = false;
    return;
  }

  Serial.print('.');
}

void MqttManager::loop() { mqtt.loop(); }

bool MqttManager::publish(const char *topic, const char *message) {
  return mqtt.publish(topic, message);
}

void MqttManager::subscribe(const char *topic) { mqtt.subscribe(topic); }

void MqttManager::onMqttMessage(char *topic, byte *payload,
                                unsigned int length) {
  if (instance == nullptr) {
    return;
  }

  instance->storeMessage(topic, payload, length);
}

void MqttManager::storeMessage(const char *topic, const byte *payload,
                               unsigned int length) {
  pendingMessage.topic = topic;
  pendingMessage.payload =
      String(reinterpret_cast<const char *>(payload), length);
  messagePending = true;

  Serial.printf("MQTT RX %s -> %s\n", topic, pendingMessage.payload.c_str());
}

bool MqttManager::getMessage(MqttIncomingMessage &message) {
  if (!messagePending) {
    return false;
  }

  message = std::move(pendingMessage);
  messagePending = false;
  pendingMessage = {};
  return true;
}
