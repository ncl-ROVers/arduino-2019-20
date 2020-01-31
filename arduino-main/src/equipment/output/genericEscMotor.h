#ifndef ESC_MOTOR
#define ESC_MOTOR

#include <Arduino.h>
#include "output.h"
#include "../../communication/communication.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/*
  The EscMotor class represents any motor controlled using PWM.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the motor.
*/
class EscMotor: public Output {

  protected:
    // Represents a motor controlled by an ESC
    Adafruit_PWMServoDriver motor;

  public:

    EscMotor (int inputPin, String partID);

    int setValue(int inputValue);

    void turnOff();
};

#endif