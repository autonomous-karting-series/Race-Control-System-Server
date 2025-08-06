#include "RCS.hpp"
#include "MQTT.hpp"
// #include "mongoose.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <threads.h>

using namespace std;

#define ENABLE_MQTT false
#define ENABLE_RCS true


int main()
{

	#if ENABLE_MQTT

	MQTTClient mqttClient(DFLT_SERVER_URI, "RCS_Server", "admin", "admin");
	mqttClient.connect();
	mqttClient.disconnect();

	#endif

	//// DONE -- Make RCS a singleton -- control initialization and destruction

	//// Create thread for RCS to run FSM on

	#if ENABLE_RCS

	using namespace RCS;

	auto& controller = RaceController::GetInstance();

	cout << get_state_string(controller.get_controller_state()) << endl;
	// controller.create_track(8);
	// Track track = controller.get_track();

	// cout << "Start\n";
	// std::vector<Sector> sectors = track.get_sectors();
	// set_flag(sectors.at(2), Flag::kGreen);

	// for (int iter = 0; iter < sectors.size(); iter++)
	// {
	//  std::cout << "Sector " << sectors.at(iter).get_id() << " flag status: " << get_flag_str<Sector>(sectors.at(iter)) << ";\n";
	// };
	// cout << "End\n";

	RaceController::Shutdown();
	#endif

	cin.get();
    return 0;
}
