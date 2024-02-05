#include "controller.hh"

ControllerPtr CONTROLLER_PTR;

void ControllerPad::setup()
{
    if (!_radio.begin())
    {
        Serial.println("Radio hardware is not responding.");
        return;
    }
    _radio.setPALevel(RF24_PA_LOW);
    _radio.openReadingPipe(1, RADIO_ADDRESSES[CONTROLLER_RADIO_ADDRESS_IDX]);
    _radio.openWritingPipe(RADIO_ADDRESSES[!CONTROLLER_RADIO_ADDRESS_IDX]);
    Serial.println("Radio hardware is ready on Controller. CH: " + String(_radio.getChannel()) + "  PA: " + String(_radio.getPALevel()));
    _syncState.radioChannel = _radio.getChannel();
    _syncState.debugMode = _debugMode;
    pinMode(_led, OUTPUT);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    BP32.setup(&onConnect, &onDisconnect);
    BP32.enableVirtualDevice(false);
}

bool ControllerPad::_checkConnection()
{
    bool connected = _ctl != nullptr && _ctl->isConnected();
    if (connected)
    {
        digitalWrite(_led, HIGH);
    }
    else
    {
        digitalWrite(_led, HIGH);
        delay(100);
        digitalWrite(_led, LOW);
        delay(100);
    }
    return connected;
}
void ControllerPad::loopSystem()
{
    BP32.update();
    _ctl = CONTROLLER_PTR;
    _radioSystem();
}
ControllerCommands ControllerPad::_getCommands()
{
    ControllerCommands cmd;
    if (_checkConnection())
    {
        cmd.throttle = _ctl->throttle();
        cmd.brake = _ctl->brake();
        cmd.button = static_cast<CONTROLLER_BUTTON>(_ctl->buttons());
        cmd.dpad = static_cast<CONTROLLER_DPAD>(_ctl->dpad());
        cmd.misc = static_cast<CONTROLLER_MISC_BUTTON>(_ctl->miscButtons());
        cmd.axisL = std::make_tuple(_ctl->axisX(), _ctl->axisY());
        cmd.axisR = std::make_tuple(_ctl->axisRX(), _ctl->axisRY());
        if (cmd.misc == CONTROLLER_MISC_BUTTON::CONTROLLER_MISC_BUTTON_SPECIAL)
        {
            _syncState.engineOn = !_syncState.engineOn;
        }
        else if (cmd.misc == CONTROLLER_MISC_BUTTON::CONTROLLER_MISC_BUTTON_BACK)
        {
            _debugMode = !_debugMode;
            _syncState.debugMode = _debugMode;
        }

        if (cmd.dpad == CONTROLLER_DPAD::CONTROLLER_DPAD_LEFT)
        {
            _syncState.radioChannel = _syncState.radioChannel - 1;
            _radio.setChannel(_syncState.radioChannel);
        }
        else if (cmd.dpad == CONTROLLER_DPAD::CONTROLLER_DPAD_RIGHT)
        {
            _syncState.radioChannel = _syncState.radioChannel + 1;
            _radio.setChannel(_syncState.radioChannel);
        }
        cmd.syncState = _syncState;
        return cmd;
    }
}
void ControllerPad::_radioSystem()
{
    _radioReceive();
    _radioSend();
}
void ControllerPad::_radioReceive()
{
    if (_checkConnection())
    {
        PlaneDataPacketToController packet;
        bool report = _radio.available();
        if (report)
        {
            _radio.read(&packet, sizeof(packet));
            if (_debugMode)
            {
                Serial.printf("Received data from plane: %d | engineOn: %d | radioChannel: %d \n", packet.speed, packet.syncState.engineOn, packet.syncState.radioChannel);
            }
        }
    }
}
void ControllerPad::_radioSend()
{
    if (_checkConnection())
    {
        ControllerCommands cmd = _getCommands();
        PlaneDataPacketToPlane packet;
        parseCMDPlane(cmd, &packet);
        bool report = _radio.write(&packet, sizeof(packet));
        if (!report)
        {
            if (_debugMode)
            {
                Serial.println("Failed to send data to plane.");
            }
        }
        else
        {
            if (_debugMode)
            {
                Serial.printf("Sent data to plane| throttle: %d | brake: %d | Ailerons: L %d R %d | engineON: %d | radioChannel: %d \n", packet.throttle, packet.brake, packet.aileronLeft, packet.aileronRight, packet.syncState.engineOn, packet.syncState.radioChannel);
            }
        }
    }
}

void onConnect(ControllerPtr ctl)
{
    Serial.printf("Controller connected: %d\n", ctl->getModelName());
    CONTROLLER_PTR = ctl;
}
void onDisconnect(ControllerPtr ctl)
{
    Serial.printf("Controller disconnected: %d\n", ctl->getModelName().c_str());
    CONTROLLER_PTR = nullptr;
}
void mapController()
{
    if (CONTROLLER_PTR != nullptr && CONTROLLER_PTR->isConnected())
    {
        ControllerPtr ctl = CONTROLLER_PTR;
        Serial.printf(
            "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
            "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
            ctl->index(),       // Controller Index
            ctl->dpad(),        // DPAD
            ctl->buttons(),     // bitmask of pressed buttons
            ctl->axisX(),       // (-511 - 512) left X Axis
            ctl->axisY(),       // (-511 - 512) left Y axis
            ctl->axisRX(),      // (-511 - 512) right X axis
            ctl->axisRY(),      // (-511 - 512) right Y axis
            ctl->brake(),       // (0 - 1023): brake button
            ctl->throttle(),    // (0 - 1023): throttle (AKA gas) button
            ctl->miscButtons(), // bitmak of pressed "misc" buttons
            ctl->gyroX(),       // Gyro X
            ctl->gyroY(),       // Gyro Y
            ctl->gyroZ(),       // Gyro Z
            ctl->accelX(),      // Accelerometer X
            ctl->accelY(),      // Accelerometer Y
            ctl->accelZ()       // Accelerometer Z
        );
    }
}