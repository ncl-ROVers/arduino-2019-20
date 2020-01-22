#ifndef OUTPUT_H
#define OUTPUT_H
#include "../../communication/communication.h"
#include <Arduino.h>

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

#endif