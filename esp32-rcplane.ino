// CHOOSE WHAT ESP32 WILL BE (BOTH ARE TRANSCEIVERS RX-TX)
// #define ESP_CONTROLLER // Controller (send commands to the plane)
#define ESP_PLANE // Plane (receive commands from the controller and send status to the controller)

#define DEBUG_MODE // Debug mode (print debug messages to the serial port)

#include "src/libs.hh"

#if defined(ESP_CONTROLLER)
ControllerPad controllerPad;
#elif defined(ESP_PLANE)
GasbPlane gasbPlane;
#endif

void setup()
{
    Serial.begin(115200);
#if defined(ESP_CONTROLLER)
    controllerPad.setup();
#elif defined(ESP_PLANE)
    gasbPlane.setup();
#endif
}

void loop()
{
#if defined(ESP_CONTROLLER)
    controllerPad.loopSystem();
#elif defined(ESP_PLANE)
    gasbPlane.loopSystem();
#endif
}
