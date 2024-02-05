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

struct PlaneDataPacketToController
{
    int speed;
    SyncState syncState;
    SensorsDataPacket sensors;
};

struct PlaneDataPacketToPlane
{
    int throttle;
    int brake;
    int aileronRight;
    int aileronLeft;
    SyncState syncState;
};
