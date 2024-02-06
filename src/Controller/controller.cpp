#include "controller.hh"

ControllerCommands CONTROLLER_COMMANDS;
ControllerPtr CONTROLLER_PTR;

void controllerSetup()
{
    pinMode(CONTROLLER_LED_PIN, OUTPUT);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    BP32.setup(&controllerOnConnect, &controllerOnDisconnect);
    BP32.enableVirtualDevice(false);
}

bool controllerCheck()
{
    BP32.update();
    bool conn = CONTROLLER_PTR != nullptr && CONTROLLER_PTR->isConnected();
    if (conn)
    {
        digitalWrite(CONTROLLER_LED_PIN, HIGH);
    }
    else
    {
        digitalWrite(CONTROLLER_LED_PIN, HIGH);
        delay(100);
        digitalWrite(CONTROLLER_LED_PIN, LOW);
        delay(100);
    }
    return conn;
}

void controllerLoop()
{
    if (controllerCheck())
    {
        controllerGetControllerCommands();
    }
}

void controllerGetControllerCommands()
{
    CONTROLLER_COMMANDS.throttle = CONTROLLER_PTR->throttle();
    CONTROLLER_COMMANDS.brake = CONTROLLER_PTR->brake();
    CONTROLLER_COMMANDS.button = static_cast<CONTROLLER_BUTTON>(CONTROLLER_PTR->buttons());
    CONTROLLER_COMMANDS.dpad = static_cast<CONTROLLER_DPAD>(CONTROLLER_PTR->dpad());
    CONTROLLER_COMMANDS.misc = static_cast<CONTROLLER_MISC_BUTTON>(CONTROLLER_PTR->miscButtons());
    CONTROLLER_COMMANDS.axisL[0] = CONTROLLER_PTR->axisX();
    CONTROLLER_COMMANDS.axisL[1] = CONTROLLER_PTR->axisY();
    CONTROLLER_COMMANDS.axisR[0] = CONTROLLER_PTR->axisRX();
    CONTROLLER_COMMANDS.axisR[1] = CONTROLLER_PTR->axisRY();
    if (CONTROLLER_COMMANDS.misc == CONTROLLER_MISC_BUTTON::CONTROLLER_MISC_BUTTON_BACK)
    {
        DEBUG_MODE = !DEBUG_MODE;
        Serial.println("ControllerPad::_getCommands: Debug mode is: " + String(DEBUG_MODE));
        delay(BUTTON_DELAY);
    }
}

void controllerOnConnect(ControllerPtr ctl)
{
    Serial.printf("Controller connected: %s\n", ctl->getModelName());
    CONTROLLER_PTR = ctl;
}

void controllerOnDisconnect(ControllerPtr ctl)
{
    Serial.printf("Controller disconnected: %d\n", ctl->getModelName().c_str());
    CONTROLLER_PTR = nullptr;
}
