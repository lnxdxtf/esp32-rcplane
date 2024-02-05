#pragma once
#include <tuple>
#include "../config.hh"
#include "controllerMap.hh"

struct ControllerCommands
{
    int throttle;
    int brake;
    CONTROLLER_BUTTON button;
    CONTROLLER_DPAD dpad;
    CONTROLLER_MISC_BUTTON misc;
    std::tuple<int, int> axisL;
    std::tuple<int, int> axisR;
    SyncState syncState;
};