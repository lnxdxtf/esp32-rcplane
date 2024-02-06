#pragma once
#include <Bluepad32.h>
#include "controllerMap.hh"
#include "controllerCommands.hh"
#include <Arduino.h>
#include "../config.hh"

extern ControllerCommands CONTROLLER_COMMANDS;
extern ControllerPtr CONTROLLER_PTR;
extern bool DEBUG_MODE;

void controllerSetup();
bool controllerCheck();
void controllerLoop();
void controllerGetControllerCommands();
void controllerOnConnect(ControllerPtr ctl);
void controllerOnDisconnect(ControllerPtr ctl);
