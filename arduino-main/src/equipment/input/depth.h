#ifndef DEPTH_H
#define DEPTH_H

#include <Arduino.h>
#include "input.h"
#include "../../communication/communication.h"

#include <Wire.h>
#include "MS5837.h"

/*
  The Depth class represents the BlueRobotics pressure sensor (measuring depth and external temperature) and sends this to the Pi using the communication class.
*/
class Depth: public Input {

  protected:
    bool initialised;
    MS5837 depthSensor;

  public:
    Depth(int inputPin, String incomingPartID);

    int getValue();
};

#endif