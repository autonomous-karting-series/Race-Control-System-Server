#include "MQTT.hpp"

#pragma region MQTTClient

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
    m_callback = std::make_unique<callback>(*m_client, m_mqtt_options);

    // m_client->set_callback(*m_callback);
    // m_client->set_message_callback(
    //     [&m_matcher](mqtt::const_message_ptr msg) {

    //     }
    // );
    std::cout << "  ...OK" << std::endl;
};

void MQTTClient::connect()
{
    try {
        std::cout << "\nConnecting..." << std::endl;
        mqtt::token_ptr conntok = m_client->connect(m_mqtt_options, nullptr, *m_callback);
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

void MQTTClient::add_topic_callback(const char* topic, void (*callback)(MQTT_Message))
{
    std::pair<std::string,std::function<void(mqtt::const_message_ptr)>> callback_pair(topic,
        [&callback](mqtt::const_message_ptr msg){
            callback(MQTT_Message(msg->get_topic().c_str(), msg->get_payload_str().c_str()));
        }
    );
    m_matcher.insert(callback_pair);

    // mqtt::const_message_ptr msg_test = mqtt::make_message("track", "tst");

    // auto match = m_matcher.matches("track");
    // match->second(msg_test);
    // m_matcher.insert(topic, [&callback](mqtt::const_message_ptr){ callback(MQTT_Message("","")); });
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

#pragma endregion

#pragma region ActionListener

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

void action_listener::on_failure(const mqtt::token& tok)
{
    std::cout << name_ << " failure";
    if (tok.get_message_id() != 0)
        std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
    std::cout << std::endl;
}

void action_listener::on_success(const mqtt::token& tok) {
    std::cout << name_ << " success";
    if (tok.get_message_id() != 0)
        std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
    auto top = tok.get_topics();
    if (top && !top->empty())
        std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
    std::cout << std::endl;
}

#pragma endregion

#pragma region Callback

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */

void callback::reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
        cli_.connect(connOpts_, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        exit(1);
    }
}
// Re-connection failure
void callback::on_failure(const mqtt::token& tok) {
    std::cout << "Connection attempt failed" << std::endl;
    if (++nretry_ > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}

// (Re)connection success
// Either this or connected() can be used for callbacks.
void callback::on_success(const mqtt::token& tok) {}

// (Re)connection success
void callback::connected(const std::string& cause) {
    std::cout << "\nConnection success" << std::endl;
    std::cout << "\nSubscribing to topic '" << "track" << "'\n"
        << "\tfor client " << cli_.get_client_id()
        << " using QoS " << QOS << "\n";
        // << "\nPress Q<Enter> to quit\n" << std::endl;

    cli_.subscribe("track", QOS, nullptr, subListener_);
}

// Callback for when the connection is lost.
// This will initiate the attempt to manually reconnect.
void callback::connection_lost(const std::string& cause) {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
        std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    nretry_ = 0;
    reconnect();
}

// Callback for when a message arrives.
void callback::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "Message arrived" << std::endl;
    std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
    std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
}

void callback::delivery_complete(mqtt::delivery_token_ptr token) {}

#pragma endregion