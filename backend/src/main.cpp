
#include <iostream>
#include <string>
#include <stdio.h>
#include <thread>

#include "RCS.hpp"
#include "MQTT.hpp"

using namespace std;

#define ENABLE_MQTT true
#define ENABLE_RCS true

int main()
{

	#if ENABLE_MQTT

	// need to make MQTTClient a singleton as well.
	MQTTClient mqttClient(DFLT_SERVER_URI, "RCS_Server", "admin", "admin");
	mqttClient.connect();


	track::Track track(4);
	track::Track_Message track_msg;
	track_msg.create_msg(track);
	mqttClient.publish_msg(track_msg);

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

	mqttClient.disconnect();

	#endif

	cin.get();
    return 0;
}
