#include "parserPacket.hh"

void parseCMDPlane(ControllerCommands cmd, PlaneDataPacketToPlane *packet)
{
    if (cmd.button == CONTROLLER_BUTTON::CONTROLLER_BUTTON_RT || cmd.button == CONTROLLER_BUTTON::CONTROLLER_BUTTON_LT)
    {
        int throttle = map(cmd.throttle, 0, 1023, PLANE_ESC_PWM_MIN, PLANE_ESC_PWM_MAX);
        int brake = map(cmd.brake, 0, 1023, 0, throttle);
        packet->throttle = throttle - brake;
    }
    int axisRX = std::get<0>(cmd.axisR);
    int axisLY = std::get<1>(cmd.axisL);

    if (abs(axisLY) > CONTROLLER_DEADZONE_AXIS || abs(axisRX) > CONTROLLER_DEADZONE_AXIS)
    {
        int pitch = map(axisLY, AXIS_RANGE_MIN, AXIS_RANGE_MAX, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
        int roll = map(axisRX, AXIS_RANGE_MIN, AXIS_RANGE_MAX, PLANE_SERVO_MIN, PLANE_SERVO_MAX);

        int aileronValLeft = constrain(pitch + roll, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
        int aileronValRight = constrain(pitch - roll, PLANE_SERVO_MIN, PLANE_SERVO_MAX);
        packet->aileronRight = aileronValRight;
        packet->aileronLeft = aileronValLeft;
    }
    else
    {
        packet->aileronRight = 90;
        packet->aileronLeft = 90;
    }
    packet->syncState = cmd.syncState;
}