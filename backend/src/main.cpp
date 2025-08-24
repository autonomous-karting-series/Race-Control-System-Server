
#include <iostream>
#include <stdio.h>
#include <thread>

#include "RCS.hpp"
#include "MQTT.hpp"

#define ENABLE_MQTT true
#define ENABLE_RCS false

int main()
{

	#if ENABLE_MQTT

	MQTTClient::Init(DFLT_SERVER_URI.c_str(), "RCS_Server", "admin", "admin");

	auto& mqttClient = MQTTClient::GetInstance();
	mqttClient.Connect();

	// mqttClient.SubscribeWithCallback("kart/+", track::message_callback);
	mqttClient.SubscribeWithCallback("track", track::message_callback);

	track::Track track(4);
	track::Track_Message track_msg;
	track_msg.create_msg(track);

	mqttClient.PublishMessage(track_msg);

	#endif

	#if ENABLE_RCS

	using namespace RCS;

	auto& controller = RaceController::GetInstance();

	std::thread controlLoop(RaceController::StartController);

	cout << get_state_string(controller.get_controller_state()) << endl;

	std::this_thread::sleep_for(5s);
	controller.create_track(2);
	std::this_thread::sleep_for(5s);
	controller.update_state(ControllerState::kCompleted);

	cin.get();
	RaceController::StopController();

	controlLoop.join();
	RaceController::Shutdown();

	#endif

	#if ENABLE_MQTT
	std::cout << "Enter \"Q\" to quit program" << std::endl;
	while (std::tolower(std::cin.get()) != 'q')
		;
	mqttClient.Disconnect();

	#endif

	// cin.get();
    return 0;
}
