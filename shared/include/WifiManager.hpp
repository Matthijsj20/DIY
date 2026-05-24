#pragma once
#include <WiFi.h>

class WifiManager {
public:
  WifiManager();
  ~WifiManager();

  /**
   * Block until WiFi is connected. Call once from setup().
   */
  void connect();

  /**
   * Non-blocking reconnect after a drop. Call repeatedly from loop().
   */
  void maintain();

  /**
   * Check if the WiFi connection is established.
   * @return True if the connection is established, false otherwise.
   */
  bool isConnected() const;

  /**
   * Disconnect from the WiFi network.
   * @return True if the disconnection was successful, false otherwise.
   */
  bool disconnect();

private:
  static constexpr unsigned long kRetryIntervalMs = 500;
  static constexpr unsigned long kConnectionTimeoutMs = 30000;

  unsigned long lastAttemptMs = 0;
  unsigned long connectionStartedMs = 0;
  bool connectionStarted = false;
  bool loggedConnected = false;
  bool wasConnected = false;
};
