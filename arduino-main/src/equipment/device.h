#ifndef DEVICE_H
#define DEVICE_H
#include "../communication/communication.h"
#include <Arduino.h>

/*
  An abstract Device (can be input or output)
*/
class Device {
    // Designed to be a generic interface for all devices.

  protected:
    String partID; // The JSON Part ID of this device

  public:
    Device(String incomingPartID);

    /*
      Control this device. This method will take e.g. a pwm thruster value representing speed and set the thruster to this value.
    */
    virtual int setValue(int inputValue);

    /*
      Get the current value of this device (EG: Temperature)
    */
    virtual int getValue();

    /*
      Get the JSON ID for this device
    */
    String getID();
};

#endif