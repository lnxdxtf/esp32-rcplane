#pragma once
#include "../planePackets.hh"
#include "../../config.hh"
#include "../../Controller/controllerCommands.hh"
#include <Arduino.h>
#include <ESP32Servo.h>

class GasbPlane
{
public:
    void setup();
    void loopSystem(ControllerCommands cmd);
    PlaneStatusPacket getPlaneStatus();
private:
    bool _engineOn = false;
    int _currentSpeed;
    Servo _aileronLeft;
    Servo _aileronRight;
    Servo _esc;
    ControllerCommands _controllerCMD;
    void _controlSystem();
    void _engineSystem();
    void _aileronSystem();
};