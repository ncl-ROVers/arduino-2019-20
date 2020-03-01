#include "temperature.h"

Temperature::Temperature(String incomingPartID){
  Wire.begin();
  // Run parent method
  partID = incomingPartID;
  maxAmp.begin(MAX31865_3WIRE);
}

int Temperature::getValue() {
  communication.bufferValue(this->partID,String(maxAmp.temperature(100, 430)));
  // Check and print any faults
  uint8_t fault = maxAmp.readFault();

  if (fault) {
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      communication.sendOnlyStatus(-14);
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      communication.sendOnlyStatus(-15);
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      communication.sendOnlyStatus(-16);
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      communication.sendOnlyStatus(-17);
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      communication.sendOnlyStatus(-18);
    }
    if (fault & MAX31865_FAULT_OVUV) {
      communication.sendOnlyStatus(-19);
    }
    maxAmp.clearFault();
  }
}