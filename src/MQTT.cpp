#include "MQTT.hpp"

MQTTClient::MQTTClient(string mqtt_uri, string client_id, string username, string password)
{
    m_mqtt_server_uri = mqtt_uri;
    m_mqtt_client_id = client_id;

    // setting defaults
    m_mqtt_options = mqtt::connect_options_builder()
                        // .mqtt_version(5)
                        .connect_timeout(TIMEOUT)
                        .keep_alive_interval(INTERVAL)
                        .clean_start(false)
                        .automatic_reconnect(true)
                        // Add more default configs
                        .finalize();

    if (!username.empty())
    {
        m_mqtt_options.set_user_name(username);
        m_mqtt_options.set_password(password);
    }

    std::cout << "Initializing for server '" << m_mqtt_server_uri << "'..." << std::endl;
    m_client = std::make_unique<mqtt::async_client>(m_mqtt_server_uri, m_mqtt_client_id);
    std::cout << "  ...OK" << std::endl;
};

void MQTTClient::connect()
{
    try {
        std::cout << "\nConnecting..." << std::endl;
        mqtt::token_ptr conntok = m_client->connect(m_mqtt_options);
        std::cout << "Waiting for the connection..." << std::endl;
        conntok->wait();
        std::cout << "  ...OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cout << "  Failed to connect" << std::endl;
        std::cerr << exc.what() << std::endl;
        return;
    }
};

void MQTTClient::disconnect()
{
    try {
        auto toks = m_client->get_pending_delivery_tokens();
        if (!toks.empty())
            std::cout << "Error: There are pending delivery tokens!" << std::endl;

        // Disconnect
        std::cout << "\nDisconnecting..." << std::endl;
        m_client->disconnect()->wait();
        std::cout << "  ...OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return;
    }
}

string MQTTClient::get_client_id() const
{
    return m_mqtt_client_id;
};

string MQTTClient::get_server_uri() const
{
    return m_mqtt_server_uri;
};

void MQTTClient::set_server_uri(const char* mqtt_uri)
{
    m_mqtt_server_uri = mqtt_uri;
};