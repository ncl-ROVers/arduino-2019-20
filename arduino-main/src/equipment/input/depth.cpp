#include "depth.h"

Depth::Depth(int inputPin){
  initialised = false;
  Wire.begin();
  // Run parent method
  if(!depthSensor.init())
  {
    // Send error message
    communication.setStatus(-4);
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
    communication.bufferValue(depthSensor.pressure());
    communication.bufferValue(depthSensor.temperature());
    communication.bufferValue(depthSensor.depth());
    communication.bufferValue(depthSensor.altitude());

  }
  else{
    // Throw error because this sensor has not yet been initialised properly
    communication.setStatus(-5);
    return -1;
  }
  return 0;
}