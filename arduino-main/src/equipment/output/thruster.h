#ifndef THRUSTER
#define THRUSTER

#include <Arduino.h>
#include "output.h"
#include "../../communication/communication.h"
#include "genericEscMotor.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/*
  The Thruster class represents a BlueRobotics T100 or T200 PWM Thruster.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the thruster.
*/
class Thruster: public EscMotor {

  public:

    Thruster (int inputPin, String partID);

    int setValue(int inputValue);

    void turnOff();
};

#endif