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
            std::cout << "Warning: There are pending delivery tokens! Continuing disconnect." << std::endl;

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

void MQTTClient::publish_msg(const MQTT_Message& msg) const
{
    mqtt::message_ptr pubmsg = mqtt::make_message(msg.topic_name, msg.message);

    std::cout << "\nSending message..." << std::endl;
    mqtt::delivery_token_ptr pubtok = m_client->publish(msg.topic_name, msg.message, 1, false);
    std::cout << "  ...with token: " << pubtok->get_message_id() << std::endl;
    std::cout << "  ...on topic: " << pubtok->get_message()->get_topic() << std::endl;
    std::cout << "  ...for message with " << pubtok->get_message()->get_payload().size()
        << " bytes" << std::endl;
    pubtok->wait_for(TIMEOUT); // waiting for send for QOS... should send on thread???
    std::cout << "  ...OK" << std::endl;
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