#ifndef RACECONTROLSERVER_RCS_H_
#define RACECONTROLSERVER_RCS_H_

#include <string>
#include <map>
#include <memory> // unique ptr
#include <iostream>
#include <assert.h>

#include "Track.hpp"

namespace RCS
{
    enum ControllerState
    {
        kUninitiated,
        kInitiated,
        kHealthy,
        kUnhealthy,
        kCompleted
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

        void start_controller();
        ControllerState& get_controller_state() const;

        void create_track(int sector_count);
        track::Track& get_track();

    private:
        RaceController();
        ~RaceController();

        ControllerState m_state;
        std::unique_ptr<track::Track> m_track;
    };
}

#endif