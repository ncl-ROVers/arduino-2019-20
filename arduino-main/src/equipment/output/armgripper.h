#ifndef ARMGRIPPER_H
#define ARMGRIPPER_H

#include <Arduino.h>
#include "output.h"
#include "../../communication/communication.h"

#include <Servo.h>

/*
  The ArmGripper class represents a BlueRobotics M100 or M200 PWM motor.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the motor.
  It also has the option of limit switches to stop movement in one direction if pressed.
*/
class ArmGripper: public Output {

  protected:
    // Represents a thruster motor opening and closing gripper
    Servo thruster;
    int leftLimit, rightLimit;

 public:

    ArmGripper(int inputPin, int limitPinLeft, int limitPinRight );

    int setValue(int inputValue);

    bool hitLeftLimit();

    bool hitRightLimit();

    void constantTask();

    void turnOff();
};

#endif