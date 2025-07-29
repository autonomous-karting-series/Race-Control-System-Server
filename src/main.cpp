#include "RCS.hpp"
#include "MQTT.hpp"
// #include "mongoose.h"

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main()
{

	MQTTClient mqttClient(DFLT_SERVER_URI, "RCS_Server", "admin", "admin");
	mqttClient.connect();
	mqttClient.disconnect();

	// RCS::RaceController controller = RCS::RaceController();

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

	cin.get();
    return 0;
}
