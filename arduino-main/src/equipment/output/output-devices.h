#include <Arduino.h>

#ifndef OUTPUTS
#define OUTPUTS

#include "../../communication/communication.h"
#include <Servo.h> // For controlling servos and thrusters

/*
  An abstract Output which has generic fields for the associated physical pin, min value, max value, stopped value, current value, and JSON ID.
  Also has generic methods for setting a new value (e.g. motor speed) with validation, getting the current value, getting the JSON ID, and switching this device off for safety.
*/
class Output {
    // Designed to be a generic interface for all output devices.

  protected:
    int maxValue; // The maximum value accepted to control this device
    int minValue; // The minimum value accepted to control this device
    int currentValue; // The current value of this device (e.g. PWM value representing speed)
    int stoppedValue; // The control value (e.g. PWM value representing speed) which means this device is stopped/not moving
    int pin; // The physical pin this is associated with
    String partID; // The JSON Part ID of this device

  public:
    Output();

    /*
      Control this device. This method will take e.g. a pwm thruster value representing speed and set the thruster to this value.
    */
    virtual int setValue(int inputValue);

    /*
      Get the current value of this device (EG: Servo position)
    */
    virtual int getValue();

    /*
      Something which needs to be run all the time
    */
    virtual void constantTask();

    /*
      Get the JSON ID for this device
    */
    String getID ();

    /*
      Switch device off - for safety
    */
    virtual void turnOff();
};

/*
  The Thruster class represents a BlueRobotics T100 or T200 PWM Thruster.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the thruster.
*/
class Thruster: public Output {

  protected:
    // Represents a thruster controlled by an ESC
    Servo thruster; // Using the Servo class because it uses the same values as our ESCs

  public:

    Thruster (int inputPin, String partID);

    int setValue(int inputValue);

    void turnOff();
};

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

    ArmGripper(int inputPin, String partID, int limitPinLeft, int limitPinRight );

    int setValue(int inputValue);

    bool hitLeftLimit();

    bool hitRightLimit();

    void constantTask();

    void turnOff();
};

/*
  The ArmRotation class represents a BlueRobotics M100 or M200 PWM motor.
  It takes values between 1100 and 1900 to control the rotation speed and direction of the motor.
  This is similar to ArmGripper except it does not have the option for limit switches.
*/
class ArmRotation: public Output {

  protected:
    // Represents a motor controlling arm rotation
    Servo servo;

 public:

    ArmRotation (int inputPin, String partID);

    int setValue(int inputValue);

    void turnOff();
};

#endif /* OUTPUTS */
