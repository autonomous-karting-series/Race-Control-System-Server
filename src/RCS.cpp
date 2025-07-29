#include "RCS.hpp"

namespace RCS
{

    RaceController::RaceController()
    {
        m_state = ControllerState::kUninitiated;
    }

    void RaceController::create_track(int sector_count)
    {
        m_track.reset(new track::Track(sector_count));
    }

    track::Track& RaceController::get_track()
    {
        return *m_track;
    }

    ControllerState RaceController::start_controller()
    {
        using namespace track;

        while(m_state != ControllerState::kCompleted)
        {
            if (m_state == ControllerState::kUninitiated)
            {
                std::cout << "Waiting to initialize, track is Red Flag";
            }

            if (m_state == ControllerState::kInitiated)
            {
                std::cout << "Initialized, setting state to healthy and track to Orange Flag -- RC Only.";
                set_flag(get_track(), Flag::kOrange);
            }

            if (m_state == ControllerState::kHealthy)
            {
                std::cout << "Controller Healthy, setting track to Green Flag -- Autonomous allowed, following race procedure";
                set_flag(get_track(), Flag::kGreen);

                // listen for status across sectors
            }

            if (m_state == ControllerState::kUnhealthy)
            {
                // kart had problem/something wrong, wait for healthy
                std::cout << "Controller Unhealthy, setting track to Orange Flag -- RC Only";
                set_flag(get_track(), Flag::kOrange);
            }

        };

        return m_state;
    }
}