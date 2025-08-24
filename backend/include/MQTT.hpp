#ifndef RACECONTROLSERVER_MQTT_H_
#define RACECONTROLSERVER_MQTT_H_

#include <iostream>
#include <functional>
#include <assert.h>
#include <unordered_map>
#include <string>
using string = std::string;

// Required for MQTT
#include <thread>
#include <atomic>
#include <chrono>

#include "mqtt/async_client.h"
#include "mqtt/topic_matcher.h"

#define topic_callback std::function<void(mqtt::const_message_ptr)>
#define topic_matcher mqtt::topic_matcher<topic_callback>

const string DFLT_SERVER_URI{"mqtt://localhost:1883"};

// const char* TOPIC{"hello"};

// const char* LWT_PAYLOAD{"Last will and testament."};

const int QOS = 1;
const int N_RETRY_ATTEMPTS = 5;
const auto NULL_MATCHER_NODE = topic_matcher().matches("");

const auto TIMEOUT = std::chrono::seconds(10);
const auto INTERVAL = std::chrono::seconds(30);

struct MQTT_Message
{
    const char* topic_name;
    string message;

    MQTT_Message(const char* topic, const char* msg = "") : topic_name(topic), message(msg) {};
    MQTT_Message() {};
};

/// MQTTClient is a wrapper class around the mqtt::async_client
/// MQTTClient is a Meyers' Singleton -- modified for creation of async_client
class MQTTClient :
    public virtual mqtt::callback,
    public virtual mqtt::iaction_listener
{
public:

    MQTTClient(const MQTTClient&) = delete;
    MQTTClient& operator=(const MQTTClient&) = delete;

    static void Init(const char* mqtt_uri, const char* client_id);
    static void Init(const char* mqtt_uri, const char* client_id, const char* username, const char* password);
    static MQTTClient& GetInstance();

    static void Connect();
    static void Disconnect();
    static void SubscribeWithCallback(const char* topic, void (*callback)(MQTT_Message));
    static void PublishMessage(const MQTT_Message& msg);

    // template <class Message>
    // static void SubscribeWithCallback(const char* topic, void (*callback)(Message));

private:
    MQTTClient(const char* mqtt_uri, const char* client_id, const char* username, const char* password);
    ~MQTTClient()
    {
        std::cout << "Deleting MQTT Client" << std::endl;
    };

    string m_mqtt_server_uri, m_mqtt_client_id;
    mqtt::connect_options m_mqtt_options = mqtt::connect_options();
    std::unique_ptr<mqtt::async_client> m_client;
    std::unordered_map<string, topic_callback> m_subscription_map;
    int m_conn_retry_;
    topic_matcher m_matcher;

    string get_client_id() const;
    string get_server_uri() const;
    void set_server_uri(const char* mqtt_uri); // May remove

    // Callback Functions
    void reconnect();
    void connected(const std::string& cause) override;
    void connection_lost(const std::string& cause) override;

    // Action Listener Functions
    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;
};

#endif