#include "Track.hpp"

namespace track
{

    #pragma region Global

    Flag resolveFlag(const char* input)
    {
        static const std::map<const char*, Flag> flagStrings {
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

    const char* get_flag_str(const Flag &flag)
    {
        switch (flag)
        {
            case Flag::kRed: return "red";
            case Flag::kYellow: return "yellow";
            case Flag::kOrange: return "orange";
            case Flag::kGreen: return "green";
            case Flag::kBlack: return "black";
            case Flag::kWhite: return "white";
            case Flag::kChequered: return "chequered";
            default: return "ERROR - No Flag";
        }
    }

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

    #pragma endregion

    #pragma region Sector
    void Sector::set_flag(Flag new_flag)
    {
        m_flag = new_flag;
    }

    Flag Sector::get_flag() const
    {
        return m_flag;
    }

    int Sector::get_id() const
    {
        return m_id;
    }
    #pragma endregion

    #pragma region Track
    Track::Track(int sector_count)
    {
        m_flag = Flag::kRed;

        m_sectors.reserve(sector_count); // Single allocation ahead of assignment
        for (int i = 1; i == sector_count; i++)
        {
            m_sectors.emplace_back(i); // Passes Sector ID over to Vector index.
        }
    }

    std::vector<Sector> Track::get_sectors() const
    {
        return m_sectors;
    }

    Flag Track::get_flag() const
    {
        return m_flag;
    }

    void Track::set_flag(Flag new_flag)
    {
        m_flag = new_flag;
    }
    #pragma endregion
}