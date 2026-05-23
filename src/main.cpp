#include <Arduino.h>

// Built-in LED on most NodeMCU-32S boards
constexpr uint8_t LED_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
}

void toggleLED() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(1000);
}
