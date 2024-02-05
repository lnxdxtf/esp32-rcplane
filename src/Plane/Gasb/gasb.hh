#pragma once
#include <ESP32Servo.h>
#include "../../config.hh"
#include "../planePackets.hh"
#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>

class GasbPlane
{
public:
    void setup();
    void loopSystem();

private:
    bool _debugMode = false;
    RF24 _radio = RF24(PLANE_RADIO_CE_PIN, PLANE_RADIO_CSN_PIN);
    Servo _aileronLeft;
    Servo _aileronRight;
    Servo _esc;
    int _currentSpeed;
    SyncState _syncState;
    PlaneDataPacketToPlane _receivedPacket;
    void _engineSystem();
    void _aileronSystem();
    void _radioSystem();
    void _radioReceive();
    void _radioSend();
};