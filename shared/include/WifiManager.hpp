#pragma once
#include <WiFi.h>

class WifiManager {
public:
  WifiManager();
  ~WifiManager();

  /**
   * Start or retry WiFi connection. Call repeatedly from loop(); non-blocking.
   */
  void connect();

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

  unsigned long lastAttemptMs = 0;
  bool connectionStarted = false;
  bool loggedConnected = false;
};
