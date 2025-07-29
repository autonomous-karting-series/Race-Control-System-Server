#ifndef RACECONTROLSERVER_MQTT_H_
#define RACECONTROLSERVER_MQTT_H_

#include <iostream>
#include <string>
using string = std::string;

// Required for MQTT
#include <thread>
#include <atomic>
#include <chrono>

#include "mqtt/async_client.h"

const string DFLT_SERVER_URI{"mqtt://localhost:1883"};

// const char* TOPIC{"hello"};

// const char* LWT_PAYLOAD{"Last will and testament."};

const int QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);
const auto INTERVAL = std::chrono::seconds(30);

class MQTTClient
{

    string m_mqtt_server_uri, m_mqtt_client_id;
    mqtt::connect_options m_mqtt_options = mqtt::connect_options();
    std::unique_ptr<mqtt::async_client> m_client;

    public:
        MQTTClient(string mqtt_uri, string client_id, string username = string(), string password = string());
        ~MQTTClient()
        {
            std::cout << "Deleting MQTT Client" << std::endl;
        };
        void connect();
        void disconnect();

        string get_client_id() const;
        string get_server_uri() const;
        void set_server_uri(const char* mqtt_uri);
};

#endif