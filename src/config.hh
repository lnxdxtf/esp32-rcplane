#pragma once
#include <cstdint>

const uint8_t RADIO_ADDRESSES[2][20] = {"PLANE_LNXDXTF", "CONTROLLER_LNXDXTF"};
const int RADIO_MAX_PAYLOAD = 32;

// ================== SYNC STATE ==================//
struct SyncState
{
    bool engineOn;
    int radioChannel;
    bool debugMode;
};

// ================== Controller Config ==================//
// Controller Radio Config
#define CONTROLLER_RADIO_CE_PIN 21
#define CONTROLLER_RADIO_CSN_PIN 22
#define CONTROLLER_RADIO_ADDRESS_IDX 0
// Controller Axis Range
#define AXIS_RANGE_MIN -512
#define AXIS_RANGE_MAX 511
// deadzone for the controller axis
#define CONTROLLER_DEADZONE_AXIS 250

// ================== Plane Config =======================//
// Plane Radio Config
#define PLANE_RADIO_CE_PIN 7
#define PLANE_RADIO_CSN_PIN 8
#define PLANE_RADIO_ADDRESS_IDX 1
// Plane Motor Pins
#define PLANE_ESC_PIN 16
#define PLANE_SERVO_RIGHT_PIN 11
#define PLANE_SERVO_LEFT_PIN 10
// Plane Motors consts
#define PLANE_ESC_PWM_MIN 1000
#define PLANE_ESC_PWM_MAX 2000
#define PLANE_SERVO_MIN 0
#define PLANE_SERVO_MAX 180