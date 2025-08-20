#ifndef RACECONTROLSERVER_MQTT_Messages_H_
#define RACECONTROLSERVER_MQTT_Messages_H_

#include <sstream>

#include "MQTT.hpp"
#include "Track.hpp"

struct Track_Message: MQTT_Message
{
    Track_Message();

    void create_msg(track::Track track);
    // void read_msg(const char* msg);
};

#endif