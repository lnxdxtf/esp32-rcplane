#pragma once
#include "../config.hh"
#include "../Controller/controller.hh"
#include "../Plane/Gasb/gasb.hh"
#include "../Plane/planePackets.hh"
#include "../Radio/radio.hh"

extern TaskHandle_t ControllerRecvTaskHandle, ControllerSendTaskHandle, PlaneRecvTaskHandle, PlaneSendTaskHandle;

void TasksSetup();

void ControllerRecvTask(void *pvParameters);
void ControllerSendTask(void *pvParameters);

void PlaneRecvTask(void *pvParameters);
void PlaneSendTask(void *pvParameters);
