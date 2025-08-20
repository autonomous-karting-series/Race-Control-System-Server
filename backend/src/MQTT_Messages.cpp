#include "MQTT_Messages.hpp"

Track_Message::Track_Message()
{
    topic_name = "track";
}

void Track_Message::create_msg(track::Track track)
{
    std::ostringstream ss;
    ss << track::get_flag_str(track) << ";";

    for (track::Sector sector : track.get_sectors())
    {
        ss << track::get_flag_str(sector) << ";";
    }

    message = ss.str();
};

// void Track_Message::read_msg(const char* msg)
// {
//     Track track(std::count(msg.begin(), msg.end(), ";"));
//     char *msg_contents = std::strtok(nullptr, ";");

//     track.set_flag(resolveFlag(msg_contents));

//     for (Sector sector : track.get_sectors())
//     {
//         set_flag(sector, resolveFlag(std::strtok(nullptr, ";")));
//     }

//     return track;
// };