#ifndef RACECONTROLSERVER_TRACK_H_
#define RACECONTROLSERVER_TRACK_H_

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

namespace track
{
    enum class Flag
    {
        kRed,
        kYellow,
        kOrange,
        kGreen,
        kBlack,
        kWhite,
        kChequered
    };

    Flag resolveFlag(std::string input)
    {
        static const std::map<std::string, Flag> flagStrings {
            { "red", Flag::kRed },
            { "yellow", Flag::kYellow },
            { "orange", Flag::kOrange },
            { "green", Flag::kGreen },
            { "black", Flag::kBlack },
            { "white", Flag::kWhite },
            { "chequered", Flag::kChequered }
        };

        auto itr = flagStrings.find(input);
        if( itr != flagStrings.end() ) {
            return itr->second;
        }
        return Flag::kRed;
    }

    class Sector
    {
    private:
        int m_id;
        Flag m_flag;

    public:
        explicit Sector(int id) : m_id(id), m_flag(Flag::kRed) {};
        explicit Sector(int id, Flag flag) : m_id(id), m_flag(flag) {}; // Not used at the moment, can be used to initiate track at a given state (maybe on live restart of RCS)
        void set_flag(Flag new_flag);
        Flag get_flag() const;
    };

    class Track
    {
    private:
        Flag m_flag;
        std::vector<Sector> m_sectors;

    public:
        explicit Track(int sector_count = 1);
        std::vector<Sector> get_sectors() const;
        void set_flag(Flag new_flag);
        Flag get_flag() const;
    };

    template <class T>
    void set_flag(T &source, Flag flag)
    {
        source.set_flag(flag);
    };

    template <class T>
    const Flag get_flag(T &source)
    {
        return source.get_flag();
    };

    template <class T>
    const std::string get_flag_str(const T &source)
    {
        return get_flag_str(get_flag(source));
    };

    const std::string get_flag_str(const Flag &flag);

    struct track_topic
    {
        std::string m_name = "track";

        std::string create_msg(Track track)
        {
            std::ostringstream ss;
            ss << get_flag_str(track) << ";";

            for (Sector sector : track.get_sectors())
                ss << get_flag_str(sector) << ";";

            return ss.str();
        };

        // Track read_msg(std::string msg)
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
    };
}

#endif