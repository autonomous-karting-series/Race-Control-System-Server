#ifndef RACECONTROLSERVER_RCS_H_
#define RACECONTROLSERVER_RCS_H_

#include <unordered_map>
#include <memory> // unique ptr
#include <iostream>
#include <thread>
#include <assert.h>

#include "Track.hpp"

namespace RCS
{
    #define DEBUG_PRINT 1

    enum ControllerState
    {
        kUninitiated,
        kInitiated,
        kHealthy_RC,
        kHealthy_Autonomous,
        kUnhealthy,
        kCompleted,
        kExited
    };

    const char* get_state_string(const ControllerState& input);

    // RaceController is following Meyers' Singleton structure
    class RaceController
    {
    public:
        // Prevent mutiple instances.
        RaceController(const RaceController&) = delete;
        RaceController& operator=(const RaceController&) = delete;

        static RaceController& GetInstance();
        static void Shutdown();
        static void StopController();
        static void StartController();

        ControllerState& get_controller_state() const;

        void create_track(int sector_count);
        void update_state(ControllerState new_state);
        track::Track& get_track();

    private:
        RaceController();
        ~RaceController();

        void debug_print(const char* msg);

        ControllerState m_state;
        std::unique_ptr<track::Track> m_track;
    };
}

#endif