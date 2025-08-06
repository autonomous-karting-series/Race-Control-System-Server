#include "RCS.hpp"

namespace RCS
{
    // Visible only inside RCS functions to manage instance
    static RaceController* s_Controller_Instance = nullptr;

    #pragma region Global

    const char* get_state_string(const ControllerState& input)
    {
        static const std::map<ControllerState, const char*> stateStrings {
            { ControllerState::kUninitiated, "Uninitiated" },
            { ControllerState::kInitiated, "Initiated" },
            { ControllerState::kHealthy, "Healthy" },
            { ControllerState::kUnhealthy, "Unhealthy" },
            { ControllerState::kCompleted, "Completed" }
        };

        auto itr = stateStrings.find(input);
        if( itr != stateStrings.end() ) {
            return itr->second;
        }
        return "Unhealthy";
    }

    #pragma endregion

    #pragma region RaceController
    RaceController::RaceController()
    {
        assert(!s_Controller_Instance);

        std::cout << "Constructing RCS Instance" << std::endl;

        m_state = ControllerState::kUninitiated;
    }

    RaceController::~RaceController()
    {
        assert(s_Controller_Instance);

        std::cout << "Deleting RCS Instance" << std::endl;
    }

    RaceController& RaceController::GetInstance()
    {
        // Lazyloading RCS Instance if null onto Heap
        if (!s_Controller_Instance)
            s_Controller_Instance = new RaceController();

        return *s_Controller_Instance;
    }

    void RaceController::Shutdown()
    {
        // Don't need shutdown on nullptr
        assert(s_Controller_Instance);

        delete &GetInstance();

        // Resetting instance to nullptr for easy debug
        s_Controller_Instance = nullptr;
    }

    ControllerState& RaceController::get_controller_state() const
    {
        assert(s_Controller_Instance);

        return s_Controller_Instance->m_state;
    }

    void RaceController::create_track(int sector_count)
    {
        assert(s_Controller_Instance);

        m_track.reset(new track::Track(sector_count));
    }

    track::Track& RaceController::get_track()
    {
        assert(s_Controller_Instance);

        return *m_track;
    }

    void RaceController::start_controller()
    {
        assert(s_Controller_Instance);

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
    }
    #pragma endregion
}