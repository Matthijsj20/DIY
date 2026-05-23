#pragma once
#include <PubSubClient.h>
#include <WiFiClient.h>

struct MqttIncomingMessage {
  String topic;
  String payload;
};

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
   * Start or retry MQTT connection. Call repeatedly from loop(); non-blocking.
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

  /**
   * Retrieve the last incoming message, if one has not been read yet.
   * @param message Out parameter filled with the message when one is available.
   * @return True if a message was retrieved, false if none is pending.
   */
  bool getMessage(MqttIncomingMessage &message);

private:
  void storeMessage(const char *topic, const byte *payload,
                    unsigned int length);

  static MqttManager *instance;

  WiFiClient wifiClient;
  PubSubClient mqtt;
  bool messagePending = false;
  MqttIncomingMessage pendingMessage;

  static constexpr unsigned long kRetryIntervalMs = 2000;

  unsigned long lastAttemptMs = 0;
  bool connectingLogged = false;
  bool loggedConnected = false;
};
