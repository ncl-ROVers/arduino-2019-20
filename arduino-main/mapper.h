#include <Arduino.h>
#include "input-devices.h"
#include "output-devices.h"
#include "constants.h"

#define T_COUNT 11 // Number of devices attached to Arduino T
#define I_COUNT 5 // Number of devices attached to Arduino I
#define M_COUNT 1 // Number of devices attached to Arduino M

#ifndef MAPPER_H
#define MAPPER_H

#test

/* ==========================Mapper========================== */
/*
  The Mapper class is a lightweight replacement for a map/dictionary structure.
  Each Arduino ID has its own array of Strings for JSON IDs, which correspond to Input or Output objects to control devices.
*/
class Mapper {
  private:
    // t for Ard_T (Thrusters)
    Output* tObjects[T_COUNT];  // Devices attached to Arduino T
    String tIDs[T_COUNT] = {"Thr_FP", "Thr_FS", "Thr_AP", "Thr_AS", "Thr_TFP", "Thr_TFS", "Thr_TAP", "Thr_TAS", "Mot_R", "Mot_G", "Mot_F"};

    // i for Ard_I (Input)
    Input* iObjects[I_COUNT];
    String iIDs[I_COUNT] = {"Sen_IMU", "Sen_Dep", "Sen_PH", "Sen_Temp", "Sen_Sonar"};

    // m for Ard_M (Micro ROV)
    Output* mObjects[M_COUNT]; // Devices attached to Arduino M
    String mIDs[M_COUNT] = {"Thr_M"};

  public:
    //Mapper();

    /*
      Assign JSON IDs to devices on this Arduino
    */
    void mapT();

    /*
      Assign JSON IDs to sensors on this Arduino
    */
    void mapI();

    /*
      Assign JSON IDs to devices on this Arduino
    */
    void mapM();

    /*
      Get the object representing an output device connected to this Arduino with the specified JSON ID
     */
    Output* getOutput(String jsonID);

    /*
      Get the object representing a senor connected to this Arduino with the specified JSON ID
     */
    Input* getInput(String jsonID);

    /*
      Get the number of sensors
     */
    int getNumberOfInputs();

    /*
      Get the number of output devices connected to this Arduino
     */
    int getNumberOfOutputs();

    /*
      Read data from all sensors and send this to the Pi
     */
    void sendAllSensors();

    /*
      Switch off all devices attached to this Arduino.
      This is primarily a safety feature to be used if no control signals are being received.
     */
    void stopOutputs();

};

#endif /* MAPPER_H */
