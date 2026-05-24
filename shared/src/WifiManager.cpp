#include "WifiManager.hpp"
#include "secrets.h"

WifiManager::WifiManager() {}

WifiManager::~WifiManager() {}

void WifiManager::connect() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("Connecting to WiFi \"%s\"", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());

  connectionStarted = true;
  loggedConnected = true;
  wasConnected = true;
}

void WifiManager::maintain() {
  const bool connected = isConnected();

  if (connected) {
    if (!loggedConnected) {
      Serial.print("WiFi connected, IP: ");
      Serial.println(WiFi.localIP());
      loggedConnected = true;
    }
    wasConnected = true;
    return;
  }

  if (wasConnected) {
    Serial.println("WiFi lost, reconnecting...");
    wasConnected = false;
    connectionStarted = false;
  }

  loggedConnected = false;

  const unsigned long now = millis();

  if (!connectionStarted) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    connectionStarted = true;
    connectionStartedMs = now;
    lastAttemptMs = now;
    return;
  }

  if (now - lastAttemptMs < kRetryIntervalMs) {
    return;
  }
  lastAttemptMs = now;

  if (now - connectionStartedMs < kConnectionTimeoutMs) {
    Serial.print('.');
    return;
  }

  Serial.println();
  Serial.println("WiFi reconnect timed out, retrying...");
  WiFi.disconnect(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  connectionStartedMs = now;
}

bool WifiManager::isConnected() const { return WiFi.status() == WL_CONNECTED; }

bool WifiManager::disconnect() {
  connectionStarted = false;
  loggedConnected = false;
  wasConnected = false;
  return WiFi.disconnect(false);
}
