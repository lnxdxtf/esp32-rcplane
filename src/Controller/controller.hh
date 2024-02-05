#pragma once
#include <Bluepad32.h>
#include "controllerMap.hh"
#include "controllerCommands.hh"
#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
#include "../config.hh"
#include "../helper/parserPacket.hh"
#include "../Plane/planePackets.hh"

#define LED_PIN 2

class ControllerPad
{
public:
    void setup();
    void loopSystem();

private:
    bool _debugMode = false;
    RF24 _radio = RF24(CONTROLLER_RADIO_CE_PIN, CONTROLLER_RADIO_CSN_PIN);
    ControllerPtr _ctl;
    SyncState _syncState;
    int _led = LED_PIN;
    bool _checkConnection();
    ControllerCommands _getCommands();
    void _radioSystem();
    void _radioSend();
    void _radioReceive();
};

void onConnect(ControllerPtr ctl);
void onDisconnect(ControllerPtr ctl);

// Use this function to map the controller on enums from controllerMap.hh
void mapController();