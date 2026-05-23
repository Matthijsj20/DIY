#include <Arduino.h>
#include <pins.hpp>

void toggleLED() {
  const bool currentState = digitalRead(INTERNAL_LED_PIN);
  digitalWrite(INTERNAL_LED_PIN, !currentState);
  Serial.println("LED TOGGLED to " + String(!currentState));
}

// Built-in LED on most NodeMCU-32S boards
void setup() {
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("BOOT");
}

void loop() {
  toggleLED();
  delay(1000);
}