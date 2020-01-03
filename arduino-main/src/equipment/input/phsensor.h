#ifndef PHSENSOR_H
#define PHSENSOR_H

#include <Arduino.h>
#include "input.h"
#include "../../communication/communication.h"

/*
  The PHSensor class represents a PH sensor which measures the water outside the ROV and sends the data to the Pi using the communication class.
*/
class PHSensor: public Input {

  protected:
    int buf[10], temp; // Store 10 samples from the sensor for an accurate average
    unsigned long int avgValue;  //Store the average value of the sensor feedback
  public:
    PHSensor(int inputPin, String incomingPartID);

    int getValue();
};

#endif