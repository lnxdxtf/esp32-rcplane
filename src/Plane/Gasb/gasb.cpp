#include "gasb.hh"

void GasbPlane::setup()
{
    // Setup radio
    if (!_radio.begin())
    {
        Serial.println("Radio hardware is not responding.");
        return;
    }
    _radio.setPALevel(RF24_PA_LOW);
    _radio.openReadingPipe(1, RADIO_ADDRESSES[PLANE_RADIO_ADDRESS_IDX]);
    _radio.openWritingPipe(RADIO_ADDRESSES[!PLANE_RADIO_ADDRESS_IDX]);
    Serial.println("Radio hardware is ready on Plane. CH: " + String(_radio.getChannel()) + "  PA: " + String(_radio.getPALevel()));
    _syncState.radioChannel = _radio.getChannel();
    _syncState.debugMode = _debugMode;
    // Setup motors
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    _aileronLeft.attach(PLANE_SERVO_LEFT_PIN);
    _aileronRight.attach(PLANE_SERVO_RIGHT_PIN);
    _esc.setPeriodHertz(50);
    _esc.attach(PLANE_ESC_PIN);
    _esc.write(PLANE_ESC_PWM_MAX / 2);
    delay(1000);
}

void GasbPlane::loopSystem()
{
    _radioSystem();
    _engineSystem();
    _aileronSystem();
}

void GasbPlane::_radioSystem()
{
    _radioReceive();
    _radioSend();
}
void GasbPlane::_radioSend()
{
    _radio.stopListening();
    PlaneDataPacketToController packet;
    packet.syncState = _syncState;
    packet.speed = _currentSpeed;
    bool report = _radio.write(&packet, sizeof(packet));
    if (!report)
    {
        if (_debugMode)
        {
            Serial.println("Failed to send packet to controller");
        }
    }
    else
    {
        if (_debugMode)
        {
            Serial.println("Sent packet to controller" + String(packet.speed));
        }
    }
}
void GasbPlane::_radioReceive()

{
    _radio.startListening();
    if (_radio.available())
    {
        PlaneDataPacketToPlane packet;
        _radio.read(&packet, sizeof(packet));
        _receivedPacket = packet;
        _debugMode = packet.syncState.debugMode;
        if (_debugMode)
        {
            Serial.println("Received packet from controller" + String(packet.throttle) + " " + String(packet.brake) + " " + String(packet.aileronLeft) + " " + String(packet.aileronRight) + " " + String(packet.syncState.engineOn));
        }
        if (packet.syncState.radioChannel != _syncState.radioChannel)
        {
            _radio.setChannel(packet.syncState.radioChannel);
            _syncState.radioChannel = packet.syncState.radioChannel;
        }
    }
}

void GasbPlane::_engineSystem()
{
    if (_receivedPacket.syncState.engineOn)
    {
        _currentSpeed = _receivedPacket.throttle;
        _esc.write(_currentSpeed);
    }
    else
    {
        _currentSpeed = PLANE_ESC_PWM_MIN;
        _esc.write(_currentSpeed);
    }
}
void GasbPlane::_aileronSystem()
{
    _aileronLeft.write(_receivedPacket.aileronLeft);
    _aileronRight.write(_receivedPacket.aileronRight);
}