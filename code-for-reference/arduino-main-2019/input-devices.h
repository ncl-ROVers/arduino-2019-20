#include <Arduino.h>

#ifndef INPUTS
#define INPUTS

//IMU
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//Depth
#include <Wire.h>
#include "MS5837.h"
//Sonar
#include "ping1d.h"
//Temperature
#include <Adafruit_MAX31865.h>

#include "communication.h"

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

/* ===========================Inputs=========================== */

/*
  The IMU class reads data from the IMU's sensors (position, acceleration, temperature) and sends this to the Pi using the communication class.
*/
class IMU: public Input {

  protected:
    bool initialised;
    Adafruit_BNO055 imu;

  public:
    IMU(int inputPin, String incomingPartID);

    int getValue();
};

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

class Temperature: public Input {

  protected:
    // Use software SPI: CS, DI, DO, CLK
    Adafruit_MAX31865 maxAmp = Adafruit_MAX31865(10, 11, 12, 13);

  public:
    Temperature(String incomingPartID);

    int getValue();
};

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
    Sonar(String incomingPartID);

    int getValue();

    /* Set parameters for sensor */
    int setParam(int index, int value);
};



#endif /* INPUTS */
