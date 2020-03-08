#ifndef ARMROTATION_H
#define ARMROTATION_H

#include <Arduino.h>
#include "output.h"
#include "../../communication/communication.h"
#include "genericEscMotor.h"


/*
  The ArmRotation class represents a BlueRobotics M100 or M200 PWM motor.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the motor.
  This is similar to ArmGripper except it does not have the option for limit switches.
*/
class ArmRotation: public EscMotor {

  protected:

  public:

    ArmRotation (int inputPin, String partID);

    int setValue(int inputValue);

    void turnOff();
};

#endif