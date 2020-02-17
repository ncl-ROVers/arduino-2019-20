#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "input.h"
#include "../../communication/communication.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/*
  The IMU class reads data from the IMU's sensors (position, acceleration, temperature) and sends this to the Pi using the communication class.
*/
class IMU: public Input {

  protected:
    bool initialised;
    Adafruit_BNO055 imu;

  public:
    IMU(int inputPin);

    int getValue();
};

#endif
