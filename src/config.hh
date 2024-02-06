#pragma once
#include <cstdint>

#define ESP_CONTROLLER // Controller
// #define ESP_PLANE // Plane

extern bool DEBUG_MODE;

#if defined(ESP_CONTROLLER)
#define RADIO_CE_PIN 21
#define RADIO_CSN_PIN 22
#else if defined(ESP_PLANE)
#define RADIO_CE_PIN 7
#define RADIO_CSN_PIN 8
#endif

#define BUTTON_DELAY 120 // ms
#define RADIO_DELAY 100   // ms
// ======================================================= //
// ================== Controller Config ================== //
// Led Controller Notification
#define CONTROLLER_LED_PIN 2

// Controller Axis Range
#define CONTROLLER_AXIS_RANGE_MIN -512
#define CONTROLLER_AXIS_RANGE_MAX 511
#define CONTROLLER_AXIS_DEADZONE 260

// Controller TASKS
const bool CONTROLLER_TASK_SEND_ACTIVE = true;
const bool CONTROLLER_TASK_RECV_ACTIVE = false;
const uint8_t CONTROLLER_TASK_RECV_PRIORITY = 1;
const uint8_t CONTROLLER_TASK_SEND_PRIORITY = 2;
// ======================================================= //
// ================== Plane Config ======================= //
// Plane Motor Pins
#define PLANE_ESC_PIN 47
#define PLANE_SERVO_RIGHT_PIN 45
#define PLANE_SERVO_LEFT_PIN 35

// Plane Motors
#define PLANE_ESC_PWM_MIN 1000
#define PLANE_ESC_PWM_MAX 2000
#define PLANE_SERVO_MIN 0
#define PLANE_SERVO_MAX 180

// Plane TASKS
const bool PLANE_TASK_SEND_ACTIVE = false;
const bool PLANE_TASK_RECV_ACTIVE = true;
const uint8_t PLANE_TASK_RECV_PRIORITY = 2;
const uint8_t PLANE_TASK_SEND_PRIORITY = 1;