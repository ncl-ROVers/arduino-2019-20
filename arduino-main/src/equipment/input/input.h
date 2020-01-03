#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

/*
  An abstract Input which has generic fields for the associated physical pin and JSON ID.
  Also has generic method for reading the values from a sensor.
*/
class Input {

  protected:
    int pin; // The physical pin this is associated with
    String partID; // The JSON Part ID of this device

  public:
    Input();

    // Get the current value of this device (EG: Temperature)
    virtual int getValue();

    // set parameters for sensor on the fly. e.g. sonar range. Each index would represent a certain property
    virtual int setParam(int index, int value);
};

#endif