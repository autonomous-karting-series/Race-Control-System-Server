#include "MQTT.hpp"

#pragma region MQTTClient_Static

static MQTTClient* s_Instance = nullptr;

void MQTTClient::Init(const char* mqtt_uri, const char* client_id, const char* username, const char* password)
{
    assert(!s_Instance);

    s_Instance = new MQTTClient(mqtt_uri, client_id, username, password);
    std::cout << "\n Adding Message Handler..." << std::endl;

    s_Instance->m_client->set_message_callback(
        [](mqtt::const_message_ptr msg) {

            auto iter = s_Instance->m_matcher.matches(msg->get_topic());

            // Loop through the MatchIterator until there are no more matches.
            while(iter != NULL_MATCHER_NODE)
            {
                iter->second(msg);
                iter++;
            }
        }
    );

    std::cout << "  ...OK" << std::endl;
}

void MQTTClient::Init(const char* mqtt_uri, const char* client_id)
{
    assert(!s_Instance);
    Init(mqtt_uri, client_id, nullptr, nullptr);
}

MQTTClient::MQTTClient(const char* mqtt_uri, const char* client_id, const char* username, const char* password)
{
    m_conn_retry_ = 0;

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

    if (username && password)
    {
        m_mqtt_options.set_user_name(username);
        m_mqtt_options.set_password(password);
    }

    std::cout << "Initializing for server '" << m_mqtt_server_uri << "'..." << std::endl;
    m_client = std::make_unique<mqtt::async_client>(m_mqtt_server_uri, m_mqtt_client_id);
    m_client->set_callback(*this);
    std::cout << "  ...OK" << std::endl;

};

MQTTClient& MQTTClient::GetInstance()
{
    assert(s_Instance);

    return *s_Instance;
}

void MQTTClient::Connect()
{
    assert(s_Instance);

    try {
        std::cout << "\nConnecting..." << std::endl;
        mqtt::token_ptr conntok = s_Instance->m_client->connect(s_Instance->m_mqtt_options);
        // mqtt::token_ptr conntok = s_Instance->m_client->connect(s_Instance->m_mqtt_options, nullptr, *s_Instance->m_callback);
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

void MQTTClient::Disconnect()
{
    assert(s_Instance);

    try {
        auto toks = s_Instance->m_client->get_pending_delivery_tokens();
        if (!toks.empty())
            std::cout << "Warning: There are pending delivery tokens! Continuing disconnect." << std::endl;

        // Disconnect
        std::cout << "\nDisconnecting..." << std::endl;
        s_Instance->m_client->disconnect()->wait();
        std::cout << "  ...OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return;
    }
}

void MQTTClient::PublishMessage(const MQTT_Message& msg)
{
    assert(s_Instance);

    mqtt::message_ptr pubmsg = mqtt::make_message(msg.topic_name, msg.message);

    std::cout << "\nSending message..." << std::endl;
    mqtt::delivery_token_ptr pubtok = s_Instance->m_client->publish(msg.topic_name, msg.message, 1, false);
    std::cout << "  ...with token: " << pubtok->get_message_id() << std::endl;
    std::cout << "  ...on topic: " << pubtok->get_message()->get_topic() << std::endl;
    std::cout << "  ...for message with " << pubtok->get_message()->get_payload().size()
        << " bytes" << std::endl;
    pubtok->wait_for(TIMEOUT); // waiting for send for QOS... should send on thread???
    std::cout << "  ...OK" << std::endl;
}

// template <class Message>
// void MQTTClient::SubscribeWithCallback(const char* topic, void (*callback)(Message))
void MQTTClient::SubscribeWithCallback(const char* topic, void (*callback)(MQTT_Message))
{
    assert(s_Instance);

    s_Instance->m_client->subscribe(topic, QOS);

    std::pair<std::string,std::function<void(mqtt::const_message_ptr)>> callback_pair(topic,
        [callback](mqtt::const_message_ptr msg){
            callback(MQTT_Message(msg->get_topic().c_str(), msg->get_payload_str().c_str()));
        }
    );
    s_Instance->m_matcher.insert(callback_pair);
    s_Instance->m_subscription_map.insert(callback_pair);
}

#pragma endregion

#pragma region MQTTClient_NonStatic

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

void MQTTClient::reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
        m_client->connect(m_mqtt_options, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        exit(1);
    }
}

void MQTTClient::on_failure(const mqtt::token& tok) {
    std::cout << "Connection attempt failed" << std::endl;
    if (++m_conn_retry_ > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}

void MQTTClient::on_success(const mqtt::token& tok) {}

void MQTTClient::connected(const std::string& cause) {
    std::cout << "\nConnection success" << std::endl;

    // If reconnecting, we need to resubscribe with callbacks.
    for (auto subscription : m_subscription_map)
    {
        m_client->subscribe(subscription.first, QOS);
    }
}

void MQTTClient::connection_lost(const std::string& cause) {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
        std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    m_conn_retry_ = 0;
    reconnect();
}

#pragma endregion