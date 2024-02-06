#pragma once
#include "../config.hh"

struct SensorsDataPacket
{
    int altitude;
    int latitude;
    int longitude;
    int temperture;
    int accelX;
    int accelY;
    int accelZ;
    int gyroX;
    int gyroY;
    int gyroZ;
    int pressure;
    int magX;
    int magY;
    int magZ;
};

struct PlaneStatusPacket
{
    int speed;
    int engineOn;
};