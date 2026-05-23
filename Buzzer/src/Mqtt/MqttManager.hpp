#pragma once
#include <PubSubClient.h>
#include <WiFiClient.h>

#include "secrets.h"

class MqttManager {
public:
  MqttManager();
  ~MqttManager();

  /**
   * Check if the MQTT client is connected to the broker.
   * @return True if the client is connected, false otherwise.
   */
  bool isConnected();

  /**
   * Connect to the MQTT broker. Blocks until connected.
   */
  void connect();

  /**
   * Callback function for when a message is received from the MQTT broker.
   * @param topic The topic of the message.
   * @param payload The payload of the message.
   * @param length The length of the message.
   */
  static void onMqttMessage(char *topic, byte *payload, unsigned int length);

  /**
   * Loop the MQTT client.
   */
  void loop();

  /**
   * Publish a message to the MQTT broker.
   * @param topic The topic to publish the message to.
   * @param message The message to publish.
   * @return True if the message was published successfully, false otherwise.
   */
  bool publish(const char *topic, const char *message);

  /**
   * Subscribe to a topic.
   * @param topic The topic to subscribe to.
   */
  void subscribe(const char *topic);

private:
  WiFiClient wifiClient;
  PubSubClient mqtt;
};