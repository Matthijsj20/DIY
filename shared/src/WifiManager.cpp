#include "WifiManager.hpp"
#include "secrets.h"

WifiManager::WifiManager() {}

WifiManager::~WifiManager() {}

void WifiManager::connect() {
  if (isConnected()) {
    if (!loggedConnected) {
      Serial.print("WiFi connected, IP: ");
      Serial.println(WiFi.localIP());
      loggedConnected = true;
    }
    return;
  }

  loggedConnected = false;

  const unsigned long now = millis();
  if (now - lastAttemptMs < kRetryIntervalMs) {
    return;
  }
  lastAttemptMs = now;

  if (!connectionStarted) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    connectionStarted = true;
    Serial.printf("Connecting to WiFi \"%s\"\n", WIFI_SSID);
    return;
  }

  Serial.print('.');
  WiFi.disconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool WifiManager::isConnected() const { return WiFi.status() == WL_CONNECTED; }

bool WifiManager::disconnect() {
  connectionStarted = false;
  loggedConnected = false;
  return WiFi.disconnect();
}
