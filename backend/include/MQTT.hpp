#ifndef RACECONTROLSERVER_MQTT_H_
#define RACECONTROLSERVER_MQTT_H_

#include <iostream>
#include <functional>
#include <string>
using string = std::string;

// Required for MQTT
#include <thread>
#include <atomic>
#include <chrono>

#include "mqtt/async_client.h"
#include "mqtt/topic_matcher.h"

const string DFLT_SERVER_URI{"mqtt://localhost:1883"};

// const char* TOPIC{"hello"};

// const char* LWT_PAYLOAD{"Last will and testament."};

const int QOS = 1;
const int N_RETRY_ATTEMPTS = 5;

const auto TIMEOUT = std::chrono::seconds(10);
const auto INTERVAL = std::chrono::seconds(30);

struct MQTT_Message
{
    const char* topic_name;
    string message;

    MQTT_Message(const char* topic, const char* msg = "") : topic_name(topic), message(msg) {};
    MQTT_Message() {};
};

class action_listener : public virtual mqtt::iaction_listener
        {
            public:
                action_listener(const char* name) : name_(name) {};

            private:
                const char* name_;

                void on_failure(const mqtt::token& tok) override;
                void on_success(const mqtt::token& tok) override;
        };

class callback :
    public virtual mqtt::callback,
    public virtual mqtt::iaction_listener
{
    public:
        callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
        : nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}

    private:
        // Counter for the number of connection retries
        int nretry_;
        // The MQTT client
        mqtt::async_client& cli_;
        // Options to use if we need to reconnect
        mqtt::connect_options& connOpts_;
        // An action listener to display the result of actions.
        action_listener subListener_;

        void reconnect();
        void on_failure(const mqtt::token& tok) override;
        void on_success(const mqtt::token& tok) override;
        void connected(const std::string& cause) override;
        void connection_lost(const std::string& cause) override;
        void message_arrived(mqtt::const_message_ptr msg) override;
        void delivery_complete(mqtt::delivery_token_ptr token) override;

};

class MQTTClient
{
    private:
        string m_mqtt_server_uri, m_mqtt_client_id;
        mqtt::connect_options m_mqtt_options = mqtt::connect_options();
        std::unique_ptr<mqtt::async_client> m_client;
        std::unique_ptr<callback> m_callback;
        mqtt::topic_matcher<std::function<void(mqtt::const_message_ptr)>> m_matcher;

    public:
        MQTTClient(string mqtt_uri, string client_id, string username = string(), string password = string());
        ~MQTTClient()
        {
            std::cout << "Deleting MQTT Client" << std::endl;
        };
        void connect();
        void disconnect();

        void publish_msg(const MQTT_Message& msg) const;
        void add_topic_callback(const char* topic, void (*callback)(MQTT_Message));

        string get_client_id() const;
        string get_server_uri() const;
        void set_server_uri(const char* mqtt_uri);

    private:
        void message_callback(mqtt::const_message_ptr message)
        {
            // for (auto& match : m_matcher.matches(message->get_topic())) {
            //     match.second(message);  // Call the matched callback
            // }
        }
};

#endif