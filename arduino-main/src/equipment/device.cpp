#include "device.h"

Device::Device(String incomingPartID) {
  partID=incomingPartID;
}

int Device::setValue(int inputValue) {
  int value = inputValue;
  // Method to set thrust capped to min and max

  return value;
  // Then set the value on the device
}

int Device::getValue() {
  return 0;
}

String Device::getID (){
  return partID;
}