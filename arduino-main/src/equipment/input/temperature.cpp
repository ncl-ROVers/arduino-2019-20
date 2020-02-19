#include "temperature.h"

Temperature::Temperature(){
  Wire.begin();
  // Run parent method
  maxAmp.begin(MAX31865_3WIRE);
}

int Temperature::getValue() {
  communication.bufferValue(maxAmp.temperature(100, 430));
  // Check and print any faults
  uint8_t fault = maxAmp.readFault();

  if (fault) {
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      communication.setStatus(-14);
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      communication.setStatus(-15);
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      communication.setStatus(-16);
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      communication.setStatus(-17);
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      communication.setStatus(-18);
    }
    if (fault & MAX31865_FAULT_OVUV) {
      communication.setStatus(-19);
    }
    maxAmp.clearFault();
  }
}