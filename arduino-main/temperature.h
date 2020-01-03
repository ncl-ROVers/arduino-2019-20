#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#include "input.h"
#include "communication.h"

#include <Adafruit_MAX31865.h>
#include <Wire.h>

class Temperature: public Input {

  protected:
    // Use software SPI: CS, DI, DO, CLK
    Adafruit_MAX31865 maxAmp = Adafruit_MAX31865(10, 11, 12, 13);

  public:
    Temperature(String incomingPartID);

    int getValue();
};

#endif