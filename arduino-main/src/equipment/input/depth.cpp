#include "depth.h"

Depth::Depth(int inputPin, String incomingPartID){
  initialised = false;
  Wire.begin();
  // Run parent method
  partID = incomingPartID;
  if(!depthSensor.init())
  {
    // Send error message
    communication.sendStatus(-4);
  }
  else{
    depthSensor.setModel(MS5837::MS5837_30BA);
    depthSensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
    initialised = true;
  }
}

int Depth::getValue() {
  if(initialised){
    depthSensor.read(); // Read current values
    communication.bufferValue(this->partID+"_Pres",String(depthSensor.pressure()));
    communication.bufferValue(this->partID+"_Temp",String(depthSensor.temperature()));
    communication.bufferValue(this->partID+"_Dep",String(depthSensor.depth()));
    communication.bufferValue(this->partID+"_Alt",String(depthSensor.altitude()));

  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.sendStatus(-5);
    return -1;
  }
  return 0;
}