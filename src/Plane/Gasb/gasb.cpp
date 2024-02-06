#include "gasb.hh"
extern bool DEBUG_MODE;
void GasbPlane::setup()
{
    // Setup motors
    ESP32PWM::allocateTimer(1);
    _aileronLeft.attach(PLANE_SERVO_LEFT_PIN);

    ESP32PWM::allocateTimer(2);
    _aileronRight.attach(PLANE_SERVO_RIGHT_PIN);

    ESP32PWM::allocateTimer(0);
    _esc.setPeriodHertz(50);
    _esc.attach(PLANE_ESC_PIN);
    _esc.write(PLANE_ESC_PWM_MAX / 2);
    delay(1000);
}

void GasbPlane::loopSystem(ControllerCommands cmd)
{
    _controllerCMD = cmd;
    _controlSystem();
    _engineSystem();
    _aileronSystem();
}

void GasbPlane::_controlSystem()
{
    if (_controllerCMD.misc == CONTROLLER_MISC_BUTTON::CONTROLLER_MISC_BUTTON_BACK)
    {
        DEBUG_MODE = !DEBUG_MODE;
        Serial.printf("DEBUG::GasbPlane::_controlSystem: Debug mode is: %s\n", DEBUG_MODE ? "ON" : "OFF");
        delay(BUTTON_DELAY);
    }
    else if (_controllerCMD.misc == CONTROLLER_MISC_BUTTON::CONTROLLER_MISC_BUTTON_START)
    {
        _engineOn = !_engineOn;
        if (DEBUG_MODE)
            Serial.printf("DEBUG::GasbPlane::_controlSystem: Engine is: %s\n", _engineOn ? "ON" : "OFF");
        delay(BUTTON_DELAY);
    }
}

void GasbPlane::_engineSystem()
{
    if (_engineOn)
    {
        int newSpeed;
        // ACCELERATE/THROTTLE
        if (_controllerCMD.button == CONTROLLER_BUTTON::CONTROLLER_BUTTON_RT)
        {
            newSpeed = map(_controllerCMD.throttle, 0, 1023, PLANE_ESC_PWM_MIN, PLANE_ESC_PWM_MAX);
        }
        // BRAKE
        else if (_controllerCMD.button == CONTROLLER_BUTTON::CONTROLLER_BUTTON_LT)
        {
            newSpeed = map(_controllerCMD.brake, 0, 1023, _currentSpeed, PLANE_ESC_PWM_MIN);
        }
        else
        {
            newSpeed = PLANE_ESC_PWM_MIN + (int)((PLANE_ESC_PWM_MAX - PLANE_ESC_PWM_MIN) * 0.24);
        }
        _currentSpeed = newSpeed;
        _esc.write(_currentSpeed);
    }
    else
    {
        _esc.write(PLANE_ESC_PWM_MIN);
    }
    if (DEBUG_MODE)
        Serial.printf("DEBUG::GasbPlane::_engineSystem: Current speed: %d\n" + _currentSpeed);
}

void GasbPlane::_aileronSystem()
{
    int axisRX = _controllerCMD.axisR[0];
    int axisLY = _controllerCMD.axisL[1];

    int aileronValLeft = _aileronLeft.read();
    int aileronValRight = _aileronRight.read();

    if (abs(axisLY) > CONTROLLER_AXIS_DEADZONE)
    {
        int pitch = map(axisLY, CONTROLLER_AXIS_RANGE_MIN, CONTROLLER_AXIS_RANGE_MAX, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
        aileronValLeft = pitch;
        aileronValRight = PLANE_SERVO_MAX - pitch + PLANE_SERVO_MIN;
    }

    if (abs(axisRX) > CONTROLLER_AXIS_DEADZONE)
    {
        int roll = map(axisRX, CONTROLLER_AXIS_RANGE_MIN, CONTROLLER_AXIS_RANGE_MAX, -abs(PLANE_SERVO_MAX - PLANE_SERVO_MIN) / 2, abs(PLANE_SERVO_MAX - PLANE_SERVO_MIN) / 2);
        aileronValLeft = constrain(aileronValLeft - roll, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
        aileronValRight = constrain(aileronValRight + roll, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
    }

    if (abs(axisRX) > CONTROLLER_AXIS_DEADZONE || abs(axisLY) > CONTROLLER_AXIS_DEADZONE)
    {
        _aileronLeft.write(PLANE_SERVO_MAX - aileronValLeft + PLANE_SERVO_MIN);
        _aileronRight.write(aileronValRight);
        if (DEBUG_MODE)
            Serial.printf("DEBUG::GasbPlane::_aileronSystem: Aileron left: %d Aileron right: %d\n", aileronValLeft, aileronValRight);
    }
}

PlaneStatusPacket GasbPlane::getPlaneStatus()
{
    PlaneStatusPacket status;
    status.engineOn = _engineOn;
    status.speed = _currentSpeed;
    return status;
}