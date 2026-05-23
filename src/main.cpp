#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <cstring>

#include "pins.hpp"
#include "secrets.h"

namespace {
constexpr char kTopic[] = "/doorbell/pressed";
constexpr unsigned long kPublishIntervalMs = 1000;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

bool doorbellPressed = false;
unsigned long lastPublishMs = 0;

void setLed(bool on) {
  digitalWrite(INTERNAL_LED_PIN, on ? HIGH : LOW);
}

bool payloadIsTrue(const uint8_t *payload, unsigned int length) {
  if (length == 4 && strncmp(reinterpret_cast<const char *>(payload), "true", 4) == 0) {
    return true;
  }
  if (length == 1 && payload[0] == '1') {
    return true;
  }
  return false;
}

void onMqttMessage(char *topic, byte *payload, unsigned int length) {
  (void)topic;
  doorbellPressed = payloadIsTrue(payload, length);
  setLed(doorbellPressed);
  Serial.printf("MQTT RX %s -> %s\n", kTopic, doorbellPressed ? "true" : "false");
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("Connecting to WiFi \"%s\"", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());
}

bool connectMqtt() {
  if (mqtt.connected()) {
    return true;
  }

  Serial.print("Connecting to MQTT broker...");
  const bool connected = strlen(MQTT_USER) > 0
                             ? mqtt.connect("esp32-doorbell", MQTT_USER, MQTT_PASS)
                             : mqtt.connect("esp32-doorbell");

  if (!connected) {
    Serial.printf(" failed (rc=%d)\n", mqtt.state());
    return false;
  }

  mqtt.subscribe(kTopic);
  Serial.println(" connected");
  Serial.printf("Subscribed to %s\n", kTopic);
  return true;
}

void publishDoorbellState() {
  const char *payload = doorbellPressed ? "true" : "false";
  if (!mqtt.publish(kTopic, payload)) {
    Serial.println("MQTT publish failed");
    return;
  }
  setLed(doorbellPressed);
  Serial.printf("MQTT TX %s -> %s\n", kTopic, payload);
}
} // namespace

void setup() {
  Serial.begin(115200);
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  setLed(false);

  connectWifi();

  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);

  while (!connectMqtt()) {
    delay(2000);
  }

  lastPublishMs = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting...");
    connectWifi();
  }

  if (!mqtt.connected()) {
    connectMqtt();
  }

  mqtt.loop();

  const unsigned long now = millis();
  if (now - lastPublishMs >= kPublishIntervalMs) {
    lastPublishMs = now;
    doorbellPressed = !doorbellPressed;
    publishDoorbellState();
  }
}
