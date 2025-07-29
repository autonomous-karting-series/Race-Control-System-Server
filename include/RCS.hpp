#ifndef RACECONTROLSERVER_RCS_H_
#define RACECONTROLSERVER_RCS_H_

#include <string>
#include <vector>
#include <memory>
#include <iostream>
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

    class RaceController
    {
    private:
        std::unique_ptr<track::Track> m_track;
        ControllerState m_state;

    public:
        RaceController();
        void create_track(int sector_count);
        track::Track& get_track();
        ControllerState start_controller();
    };
}

#endif