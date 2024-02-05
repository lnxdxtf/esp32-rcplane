#pragma once
#include "../Plane/planePackets.hh"
#include "../Controller/controllerCommands.hh"
#include "../Controller/controllerMap.hh"
#include "../config.hh"
#include <Arduino.h>

void parseCMDPlane(ControllerCommands cmd, PlaneDataPacketToPlane *packet);