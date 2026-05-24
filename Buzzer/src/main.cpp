#include <Arduino.h>
#include <cstring>

#include "ConnectionStatusLeds.hpp"
#include "WifiManager.hpp"
#include "MqttManager.hpp"
#include "pins.hpp"

namespace {
WifiManager wifiManager;
MqttManager mqttManager;
ConnectionStatusLeds statusLeds(LED_RED_PIN, LED_GREEN_PIN);

bool mqttSubscribed = false;
bool internalLedOn = false;
unsigned long internalLedOffAt = 0;

constexpr unsigned long kInternalLedOnDurationMs = 5000;

void updateInternalLed() {
  if (internalLedOn && millis() >= internalLedOffAt) {
    digitalWrite(BUZZER_PIN, LOW);
    internalLedOn = false;
  }
}

void onDoorbellPressed() {
  if (internalLedOn) {
    return;
  }
  digitalWrite(BUZZER_PIN, HIGH);
  internalLedOn = true;
  internalLedOffAt = millis() + kInternalLedOnDurationMs;
}

} // namespace

void setup() {
  Serial.begin(115200);
  statusLeds.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  wifiManager.connect();
}

void loop() {
  wifiManager.maintain();
  if (wifiManager.isConnected()) {
    mqttManager.connect();
    if (mqttManager.isConnected()) {
      if (!mqttSubscribed) {
        mqttManager.subscribe("/doorbell/pressed");
        mqttSubscribed = true;
      }
    } else {
      mqttSubscribed = false;
    }
  } else {
    mqttSubscribed = false;
  }

  statusLeds.update(wifiManager.isConnected(), mqttManager.isConnected());

  mqttManager.loop();
  updateInternalLed();

  MqttIncomingMessage message;
  if (mqttManager.getMessage(message)) {
    if (message.topic == "/doorbell/pressed") {
      onDoorbellPressed();
    }
  }
}
