#include "RCS.hpp"

namespace RCS
{
    // Visible only inside RCS functions to manage instance
    static RaceController* s_Controller_Instance = nullptr;

    #pragma region Global

    const char* get_state_string(const ControllerState& input)
    {
        static const std::unordered_map<ControllerState, const char*> stateStrings {
            { ControllerState::kUninitiated, "Uninitiated" },
            { ControllerState::kInitiated, "Initiated" },
            { ControllerState::kHealthy_RC, "Healthy_RC" },
            { ControllerState::kHealthy_Autonomous, "Healthy_Autonomous" },
            { ControllerState::kUnhealthy, "Unhealthy" },
            { ControllerState::kCompleted, "Completed" },
            { ControllerState::kExited, "Exited" }
        };

        auto itr = stateStrings.find(input);
        if( itr != stateStrings.end() ) {
            return itr->second;
        }
        return "Ended";
    }

    #pragma endregion

    #pragma region Controller Static
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

    void RaceController::StopController()
    {
        assert(s_Controller_Instance);

        s_Controller_Instance->m_state = kExited;
    }

    /// @brief Start Controller will block main thread, execute on a different thread.
    void RaceController::StartController()
    {
        assert(s_Controller_Instance);

        using namespace track;
        using namespace std::literals::chrono_literals;

        while(s_Controller_Instance->m_state != kCompleted && s_Controller_Instance->m_state != kExited)
        {
            // Sleep to reduce overloading thread
            std::this_thread::sleep_for(1s);

            // Execute according to state -- continue to continue looping and avoid early exits
            switch (s_Controller_Instance->m_state)
            {
                case kUninitiated:
                    s_Controller_Instance->debug_print("Waiting to initialize, no track defined.");

                    if(s_Controller_Instance->m_track)
                        s_Controller_Instance->update_state(kInitiated);

                    continue;

                case kInitiated:
                    assert(s_Controller_Instance->m_track);

                    // We do not want to allow anything on the track until it is healthy.
                    s_Controller_Instance->debug_print("Track initialized, waiting to enable karts.");

                    // Can configure track in this state or do nothing.
                    continue;

                case kHealthy_RC:
                    assert(s_Controller_Instance->m_track);

                    s_Controller_Instance->debug_print("Controller Healthy: RC Only Allowed, following safety procedures.");

                    if (get_flag(s_Controller_Instance->get_track()) != Flag::kOrange)
                        set_flag(s_Controller_Instance->get_track(), Flag::kOrange);

                    // listen for status across sectors
                    continue;

                case kHealthy_Autonomous:
                    assert(s_Controller_Instance->m_track);

                    s_Controller_Instance->debug_print("Controller Healthy: Autonomous allowed, following safety procedures.");

                    if (get_flag(s_Controller_Instance->get_track()) != Flag::kGreen)
                        set_flag(s_Controller_Instance->get_track(), Flag::kGreen);

                    // listen for status across sectors
                    continue;

                case kUnhealthy:
                    assert(s_Controller_Instance->m_track);

                    // kart had problem/something wrong, wait for healthy
                    s_Controller_Instance->debug_print("Controller Unhealthy: RC Only.");

                    if (get_flag(s_Controller_Instance->get_track()) != Flag::kOrange)
                        set_flag(s_Controller_Instance->get_track(), Flag::kOrange);

                    continue;

                case kCompleted:
                    s_Controller_Instance->debug_print("Controller completed, exiting RCS Control Loop.");
                    continue;

                default:
                    break;
            }

            // Exit the loop in the case that the while loop doesn't exit correctly
            std::cout << "Something went wrong, exiting RCS Control Loop." << std::endl;
            break;
        };
    }
    #pragma endregion

    #pragma region Controller Non-Static
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

    void RaceController::update_state(ControllerState new_state)
    {
        assert(s_Controller_Instance);

        m_state = new_state;
    }

    track::Track& RaceController::get_track()
    {
        assert(s_Controller_Instance);

        return *m_track;
    }

    void RaceController::debug_print(const char* msg)
    {
        #if DEBUG_PRINT

        std::cout << msg << std::endl;

        #endif
    }

    #pragma endregion
}