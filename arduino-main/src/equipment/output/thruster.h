#ifndef THRUSTER
#define THRUSTER

#include <Arduino.h>
#include "output.h"
#include "../../communication/communication.h"

#include <Servo.h>

/*
  The Thruster class represents a BlueRobotics T100 or T200 PWM Thruster.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the thruster.
*/
class Thruster: public Output {

  protected:
    // Represents a thruster controlled by an ESC
    Servo thruster; // Using the Servo class because it uses the same values as our ESCs

  public:

    Thruster (int inputPin);

    int setValue(int inputValue);

    void turnOff();
};

#endif