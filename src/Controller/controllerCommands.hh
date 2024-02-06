#pragma once
#include "../config.hh"
#include "controllerMap.hh"

struct ControllerCommands
{
    uint16_t throttle;
    uint16_t brake;
    uint8_t button;
    uint8_t dpad;
    uint8_t misc;
    int16_t axisL[2];
    int16_t axisR[2];
};