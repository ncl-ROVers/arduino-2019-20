#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>
#include "input.h"
#include "../../communication/communication.h"

#include "ping1d.h"

/*
  The Sonar class represents the BlueRobotics Sonar (measuring distance) and sends this to the Pi using the communication class.
*/
class Sonar: public Input {
    // Designed to be a generic interface for all output devices.

  protected:
    bool initialised;
    Ping1D sonar { Serial1 }; // sonar object
    int sonStart, sonLen;
    
  public:
    Sonar();

    int getValue();

    /* Set parameters for sensor */
    int setParam(int index, int value);
};

#endif