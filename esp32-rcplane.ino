#include "src/libs.hh"
bool DEBUG_MODE = false;

#if defined(ESP_CONTROLLER)
int addressIDX = 0;
#elif defined(ESP_PLANE)
GasbPlane gasbPlane;
int addressIDX = 1;
#endif

void setup()
{
    Serial.begin(115200);
    radioSetup(addressIDX);
#if defined(ESP_CONTROLLER)
    controllerSetup();
#elif defined(ESP_PLANE)
    gasbPlane.setup();
#endif
    TasksSetup();
}

void loop()
{
}
