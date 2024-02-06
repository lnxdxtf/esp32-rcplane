#include "tasks.hh"

extern GasbPlane gasbPlane;
extern bool DEBUG_MODE;
extern uint8_t CONTROLLER_TASK_SEND_PRIORITYl;
extern uint8_t CONTROLLER_TASK_RECV_PRIORITYl;
extern uint8_t PLANE_TASK_RECV_PRIORITYl;
extern uint8_t PLANE_TASK_SEND_PRIORITYl;
TaskHandle_t ControllerRecvTaskHandle, ControllerSendTaskHandle, PlaneRecvTaskHandle, PlaneSendTaskHandle;
void TasksSetup()
{
    Serial.println("Creating Tasks");
#if defined(ESP_CONTROLLER)
    Serial.println("Creating tasks for Controller");
    if (CONTROLLER_TASK_RECV_ACTIVE)
    {
        Serial.println("Creating ControllerRecvTask");
        xTaskCreatePinnedToCore(ControllerRecvTask, "ControllerRecvTask", 10000, NULL, CONTROLLER_TASK_RECV_PRIORITY, &ControllerRecvTaskHandle, 0);
    }
    if (CONTROLLER_TASK_SEND_ACTIVE)
    {
        Serial.println("Creating ControllerSendTask");
        xTaskCreatePinnedToCore(ControllerSendTask, "ControllerSendTask", 10000, NULL, CONTROLLER_TASK_SEND_PRIORITY, &ControllerSendTaskHandle, 1);
    }
#elif defined(ESP_PLANE)
    Serial.println("Creating tasks for Plane");
    if (PLANE_TASK_RECV_ACTIVE)
    {
        Serial.println("Creating PlaneRecvTask");
        xTaskCreatePinnedToCore(PlaneRecvTask, "PlaneRecvTask", 10000, NULL, PLANE_TASK_RECV_PRIORITY, &PlaneRecvTaskHandle, 0);
    }
    if (PLANE_TASK_SEND_ACTIVE)
    {
        Serial.println("Creating PlaneSendTask");
        xTaskCreatePinnedToCore(PlaneSendTask, "PlaneSendTask", 10000, NULL, PLANE_TASK_SEND_PRIORITY, &PlaneSendTaskHandle, 1);
    }
#endif
}

void ControllerRecvTask(void *pvParameters)
{
    while (true)
    {
        radio.startListening();
        if (radio.available())
        {
            uint8_t buf[sizeof(PlaneStatusPacket)];
            radio.read(&buf, sizeof(buf));
            PlaneStatusPacket planeStatus;
            memcpy(&planeStatus, &buf, sizeof(PlaneStatusPacket));

            if (DEBUG_MODE)
            {
                Serial.println("DEBUG::ControllerRecvTask: Received data from plane: " + String(planeStatus.engineOn ? "ON" : "OFF") + " " + String(planeStatus.speed));
            }
        }
    }
}

void ControllerSendTask(void *pvParameters)
{
    while (true)
    {
        if (controllerCheck())
        {
            controllerLoop();
            radio.stopListening();
            uint8_t buf[sizeof(CONTROLLER_COMMANDS)];
            memcpy(buf, &CONTROLLER_COMMANDS, sizeof(CONTROLLER_COMMANDS));
            bool report = radio.write(&buf, sizeof(buf));
            if (DEBUG_MODE)
            {
                String status = report ? "OK" : "FAIL";
                Serial.printf("DEBUG::ControllerSendTask: throttle: %d brake: %d button: %d dpad: %d misc: %d axisL:(%d , %d) axisR:(%d , %d)\n",
                              CONTROLLER_COMMANDS.throttle, CONTROLLER_COMMANDS.brake, CONTROLLER_COMMANDS.button, CONTROLLER_COMMANDS.dpad, CONTROLLER_COMMANDS.misc,
                              CONTROLLER_COMMANDS.axisL[0], CONTROLLER_COMMANDS.axisL[1], CONTROLLER_COMMANDS.axisR[0], CONTROLLER_COMMANDS.axisR[1]);
                Serial.printf("DEBUG::ControllerSendTask: Send (%d bytes) Status %s\n", sizeof(CONTROLLER_COMMANDS), status);
            }
            delay(RADIO_DELAY);
        }
        vTaskDelay(1);
    }
}

void PlaneRecvTask(void *pvParameters)
{
    while (true)
    {
        radio.startListening();
        if (radio.available())
        {
            uint8_t buf[sizeof(ControllerCommands)];
            radio.read(&buf, sizeof(buf));
            ControllerCommands cmds;
            memcpy(&cmds, &buf, sizeof(ControllerCommands));
            gasbPlane.loopSystem(cmds);
        }
        vTaskDelay(1);
    }
}

void PlaneSendTask(void *pvParameters)
{
    while (true)
    {
        radio.stopListening();
        PlaneStatusPacket planeStatus = gasbPlane.getPlaneStatus();
        uint8_t buf[sizeof(PlaneStatusPacket)];
        memcpy(buf, &planeStatus, sizeof(PlaneStatusPacket));
        bool report = radio.write(&buf, sizeof(buf));
        if (!report && DEBUG_MODE)
        {
            Serial.println("PlaneSendTask: Failed to send data");
        }
    }
}
