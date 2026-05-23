#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <pins.hpp>

void toggleLED() {
  const bool currentState = digitalRead(INTERNAL_LED_PIN);
  digitalWrite(INTERNAL_LED_PIN, !currentState);
  Serial.println("LED TOGGLED to " + String(!currentState));
}

void onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  (void)mac;
  (void)data;
  (void)len;
  toggleLED();
}

void setup() {
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("BOOT");

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);

  Serial.print("ESP-NOW client ready. MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}
