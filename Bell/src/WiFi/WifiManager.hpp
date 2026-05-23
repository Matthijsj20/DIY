#pragma once
#include <WiFi.h>
#include "secrets.h"

class WifiManager {
public:
  WifiManager();
  ~WifiManager();

  /**
   * Connect to the WiFi network.
   * @return True if the connection was successful, false otherwise.
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
};