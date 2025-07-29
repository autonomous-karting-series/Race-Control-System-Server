#include "Track.hpp"

namespace track
{

    // Global
    const std::string get_flag_str(const Flag &flag)
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

    // Sector
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

    // Track
    Track::Track(int sector_count)
    {
        m_flag = Flag::kRed;

        for (int i = 0; i < sector_count; i++)
        {
            m_sectors.push_back(Sector(i));
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
}